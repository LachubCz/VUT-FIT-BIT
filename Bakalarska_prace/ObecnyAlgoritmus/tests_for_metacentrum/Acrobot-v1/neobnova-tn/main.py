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
observetime = 200                            # Delka pozorovani
episodes = 2500                              # Pocet epizod
games = 20                                   # Pocet her
scores = []                                  # Pole pro ulozeni vysledku na analyzu
episodes_numbers = []                        # Pole pro ulozeni cisel epizod na analyzu
best_avg_score = float("-inf")               # Promenna pro ukladani nejlepsiho prumerneho vysledku
task = Task("Acrobot-v1")
#task.env = wrappers.Monitor(env, '/home/lachubcz/tmp/cartpole-experiment-1', force=True)

rand_agent_replay(task, 10000, 500)

#task.agent.load_model_weights("./DDQN-MountainCar-v0.h5")                # Nacteni vah NN
#task.agent.updateTargetNet()                                             # Nacteni NN do netTarget

for eps in range(episodes):
    state = task.env.reset()                                              # Resetovani prostredi
    
    for t in range(observetime):
        state = np.reshape(state, (1, task.env_state_size))               # Formatovani
        action = task.agent.getActionWE(state)                            # Ziskani akce
        nextState, reward, done, info = task.env.step(action)             # Provedeni akce

        task.agent.remember(state, action, reward, nextState, done)       # Ulozeni stavu do pameti
        task.agent.trainDQN()                                             # Trenovani pameti
        
        state = nextState                                                 # Zmena stavu
        
        task.agent.epsilonActulization()                                  # Aktualizace epsilon
        if done:                                                          # Konec epizody
            task.agent.updateTargetNet()                                  # Aktualizace target site
            
            score = score_estimate(task, 1)                               # Vypocet aktualniho skore
            scores.append(score)                                          # Ulozeni aktualniho skore
            episodes_numbers.append(eps)                                  # Ulozeni aktualniho cisla epizody

            print("Episode: {}/{}, epsilon: {:.2}, score: {}".format(eps, episodes, task.agent.currentEpsilon, score))

            if score >= task.solved_score:
                if score_estimate(task, 10) >= task.solved_score:
                    task.test(eps, scores, episodes_numbers)

            if eps % 25 == 0:
                #avg_score = score_estimate(task, games)                   # Vypocet aktualniho skore
                
                #if avg_score > best_avg_score and eps != 0:
                #    best_avg_score = avg_score
                #    agent_replay(task,  10000, 500)
                
                task.agent.save_model_weights("./{}-{}.h5" .format(task.name, eps))       # Ulozeni site

            break

task.agent.save_model_weights("./{}-last.h5" .format(task.name, eps))                    # Ulozeni site
scattergrams_graph(scores, episodes_numbers, "learning-cp0-pgraph.png")
line_graph(scores, episodes_numbers, "learning-cp0-lgraph-i1.png")
line_graph_i5(scores, episodes_numbers, "learning-cp0-lgraph-i5.png")
line_graph_i11(scores, episodes_numbers, "learning-cp0-lgraph-i11.png")
line_graph_i101(scores, episodes_numbers, "learning-cp0-lgraph-i101.png")