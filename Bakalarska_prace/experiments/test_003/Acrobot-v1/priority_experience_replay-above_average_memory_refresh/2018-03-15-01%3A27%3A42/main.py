#!/usr/bin/env python
import copy                                   # Kopirovani prvku
import numpy as np                            # Matematicke operace s maticemi
import random                                 # Vyber nahodnych prvku
import h5py                                   # Ukladani vah site
import gym                                    # Prostredi Open AI Gym
from gym import wrappers                      # Nahravani zaznamu

#%matplotlib inline
#import matplotlib.pyplot as plt
from collections import deque                 # Pamet
#from __future__ import division              # Deleni realnych cisel (kvuli nizsi verzi Pythonu 2.6)
#from tqdm import tnrange, tqdm_notebook      # Progress bar
from profiling import * #profiling - @do_profile(follow=[method, ])
from playing import *
from visualization import *
from agent import *
from task import *
from memory import *

#Keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten
from keras import regularizers
from keras import optimizers
from keras import losses
from keras import metrics

import os
os.environ['TF_CPP_MIN_LOG_LEVEL']='2'

#Tensorflow session
import tensorflow as tf                     # Knihovna Tensorflow
from keras.backend.tensorflow_backend import set_session
config = tf.ConfigProto(intra_op_parallelism_threads=1, inter_op_parallelism_threads=1)
set_session(tf.Session(config=config))

# Parametry
episodes = 2500                              # Pocet epizod
games = 20                                   # Pocet her
scores = []                                  # Pole pro ulozeni vysledku na analyzu
episodes_numbers = []                        # Pole pro ulozeni cisel epizod na analyzu
last_avg_score = float("-inf")               # Promenna pro ukladani nejlepsiho prumerneho vysledku
task = Task("Acrobot-v1")
#task.env = wrappers.Monitor(env, '/home/lachubcz/tmp/cartpole-experiment-1', force=True)

rand_agent_replay(task, 10000, task.max_steps)

#task.agent.load_model_weights("./DDQN-MountainCar-v0.h5")                # Nacteni vah NN
#task.agent.updateTargetNet()                                             # Nacteni NN do netTarget

for eps in range(episodes):
    state = task.env.reset()                                              # Resetovani prostredi
    
    for t in range(task.max_steps):
        state = np.reshape(state, (1, task.env_state_size))               # Formatovani
        action = task.agent.getActionWE(state)                            # Ziskani akce
        nextState, reward, done, info = task.env.step(action)             # Provedeni akce

        task.agent.remember(state, action, reward, nextState, done, rand_agent = False)       # Ulozeni stavu do pameti
        task.agent.trainDQN()                                             # Trenovani pameti
        
        state = nextState                                                 # Zmena stavu
        
        task.agent.epsilonActulization()                                  # Aktualizace epsilon
        if done:                                                          # Konec epizody
            #print("{}".format(task.agent.memory.priority_tree))
            score = score_estimate(task, 1)                               # Vypocet aktualniho skore
            scores.append(score)                                          # Ulozeni aktualniho skore
            episodes_numbers.append(eps)                                  # Ulozeni aktualniho cisla epizody

            print("Episode: {}/{}, epsilon: {:.2}, score: {}".format(eps, episodes, task.agent.currentEpsilon, score))

            if score >= task.solved_score:
                task.test(eps, scores, episodes_numbers)

            if eps % 25 == 0:
                #avg_score = score_estimate(task, games)                   # Vypocet aktualniho skore
                if eps != 0:
                    #last_avg_score = avg_score
                    before = task.agent.memory.priority_tree[0]
                    added = agent_replay(task, 100, task.max_steps)
                    after = task.agent.memory.priority_tree[0]
                    print ("Added {} new memories. Value of priority_tree was {} and now is {}." .format(added, before, after))
                #else:
                    #last_avg_score = avg_score
                task.agent.save_model_weights("./{}-{}.h5" .format(task.name, eps))       # Ulozeni site

            task.agent.updateTargetNet()                                  # Aktualizace target site
            break

task.agent.save_model_weights("./{}-last.h5" .format(task.name, eps))                    # Ulozeni site
point_graph(scores, episodes_numbers, "{}-point_graph.png" .format(task.name))
gaussian_graph(scores, episodes_numbers, "{}-gaussian_graph.png" .format(task.name))
combined_graph(scores, episodes_numbers, "{}-combined_graph.png" .format(task.name))