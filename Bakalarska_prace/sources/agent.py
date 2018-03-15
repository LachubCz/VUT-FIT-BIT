import numpy as np
import tensorflow as tf
import random
import h5py
from collections import deque
from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten
from keras import regularizers
from keras import optimizers
from keras import losses
from keras import metrics
from keras import backend as K
from memory import *

class Agent:
    def __init__(self, state_size, action_size):
        self.startEpsilon = 1                           # Pravdepodobnost konani nahodneho tahu na zacatku
        self.endEpsilon = 0.005                         # Pravdepodobnost konani nahodneho tahu na konci
        self.currentEpsilon = self.startEpsilon         # Soucasna pravdepodobnost konani nahodneho tahu
        self.epsilonDiminution = (self.startEpsilon - self.endEpsilon) / 50000#(self.startEpsilon - self.endEpsilon)/(episodes * 2)                  # Hodnota snizovani epsilonu
        self.gamma = 0.99                               # Discount faktor
        self.minibatchSize = 64                         # Velikost minibatche
        self.action_size = action_size                  # Pocet vstupu do prostredi
        self.state_size = state_size                    # Pocet vystupu z prostredi
        self.learningRate = 0.001                       # Learning rate
        self.fractionUpdate = 0.125
        self.memory_size = 10000                        # Velikost Replay memory
        self.memory = Memory(self.memory_size)
        
        self.net = self.make_model()
        self.netTarget = self.make_model()
        self.updateTargetNet()

    def make_model(self):
        net = Sequential()

        net.add(Dense(32, activation="relu", input_shape=(self.state_size,), kernel_initializer="he_uniform"))
        net.add(Dense(16, activation="relu", kernel_initializer="he_uniform"))
        net.add(Dense(self.action_size, activation="linear", kernel_initializer="he_uniform"))

        net.summary()

        net.compile(loss=losses.mean_squared_error, optimizer=optimizers.RMSprop(lr=self.learningRate), metrics=['accuracy'])

        return net
    
    def updateTargetNet(self):
        self.netTarget.set_weights(self.net.get_weights())
    
    def updateTargetNetPartially(self):
        weights = self.net.get_weights()
        weightsTarget = self.netTarget.get_weights()
        
        for i in range(len(weightsTarget)):
            weightsTarget[i] = weights[i] * self.fractionUpdate + weightsTarget[i] * (1 - self.fractionUpdate)
            
        self.netTarget.set_weights(weightsTarget)
        
    def get_error(self, state, action, reward, next_state, done):
        target_f = self.net.predict(state)
        obs_error = target_f[0][action]

        if done == 1:
            target_f[0][action] = reward
        else:
            aNet = self.netTarget.predict(np.array([next_state]))[0]
            target_f[0][action] = reward + self.gamma * np.max(aNet)

        obs_error = abs(obs_error - target_f[0][action])

        return obs_error

    def remember(self, state, action, reward, next_state, done, rand_agent):
        if rand_agent == True:
            obs_error = abs(reward)
        else:
            obs_error = self.get_error(state, action, reward, next_state, done)

        self.memory.add_observation((state, action, reward, next_state, done), obs_error)
        
    def clear_memory(self):
        self.memory.clear()
        
    def epsilonActulization(self):
        if self.currentEpsilon > self.endEpsilon:
            if (self.currentEpsilon - self.epsilonDiminution) > self.endEpsilon:
                self.currentEpsilon = self.currentEpsilon - self.epsilonDiminution
            else:
                self.currentEpsilon = self.endEpsilon
    
    def getActionWE(self, state):
        if np.random.rand() <= self.currentEpsilon:
            return np.random.randint(0, self.action_size, size=1)[0]
        else:
            Q = self.net.predict(state)
            return np.argmax(Q)

    def get_action(self, state):
        Q = self.net.predict(state)
        return np.argmax(Q)
        
    def resetEpsilon(self):
        self.currentEpsilon = self.startEpsilon
    
    def trainDQN(self):
        if self.memory.length >= self.minibatchSize:
            minibatch = self.memory.sample(self.minibatchSize)
        else:
            return

        state = np.vstack([i[1][0] for i in minibatch])
        action = [i[1][1] for i in minibatch]
        reward = [i[1][2] for i in minibatch]
        nextState = np.vstack([i[1][3] for i in minibatch])
        done = [i[1][4] for i in minibatch]

        target_fs = np.zeros((self.minibatchSize, self.action_size))
        errors = np.zeros(self.minibatchSize)

        for i in range(0, self.minibatchSize):
            target_f = self.net.predict(np.array([state[i]]))
            errors[i] = target_f[0][action[i]]

            if done[i] == 1:
                target_f[0][action[i]] = reward[i]
            else:
                aNet = self.netTarget.predict(np.array([nextState[i]]))[0]
                target_f[0][action[i]] = reward[i] + self.gamma * np.max(aNet)

            errors[i] = abs(errors[i] - target_f[0][action[i]])
            for e in range(self.action_size):
                target_fs[i][e] = target_f[0][e]
        
        self.net.fit(state, target_fs, epochs=1, verbose=0)
        self.memory.update_minibatch(minibatch, errors)

    def trainDDQN(self):
        if self.memory.length >= self.minibatchSize:
            minibatch = self.memory.sample(self.minibatchSize)
        else:
            return

        state = np.vstack([i[1][0] for i in minibatch])
        action = [i[1][1] for i in minibatch]
        reward = [i[1][2] for i in minibatch]
        nextState = np.vstack([i[1][3] for i in minibatch])
        done = [i[1][4] for i in minibatch]

        target_fs = np.zeros((self.minibatchSize, self.action_size))
        errors = np.zeros(self.minibatchSize)

        for i in range(0, self.minibatchSize):
            target_f = self.net.predict(np.array([state[i]]))
            errors[i] = target_f[0][action[i]]

            if done[i] == 1:
                target_f[0][action[i]] = reward[i]
            else:
                aNet = self.net.predict(np.array([nextState[i]]))[0]
                tNet = self.netTarget.predict(np.array([nextState[i]]))[0]
                target_f[0][action[i]] = reward[i] + self.gamma * tNet[np.argmax(aNet)]

            errors[i] = abs(errors[i] - target_f[0][action[i]])
            for e in range(self.action_size):
                target_fs[i][e] = target_f[0][e]
        
        self.net.fit(state, target_fs, epochs=1, verbose=0)
        self.memory.update_minibatch(minibatch, errors)
    
    def load_model_weights(self, name):
        self.net.load_weights(name)

    def save_model_weights(self, name):
        self.net.save_weights(name)