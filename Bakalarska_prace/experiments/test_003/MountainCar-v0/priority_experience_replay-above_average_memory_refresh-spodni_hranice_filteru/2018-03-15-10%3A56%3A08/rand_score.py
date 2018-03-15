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

task = Task("MountainCar-v0")
print("100: {}".format(rand_score_estimate(task, 100)))
print("1000: {}".format(rand_score_estimate(task, 1000)))
print("10000: {}".format(rand_score_estimate(task, 10000)))
print("100000: {}".format(rand_score_estimate(task, 100000)))