import numpy as np
import random
import h5py
from collections import deque
from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten
from keras import regularizers
from keras import optimizers
from keras import losses
from keras import metrics

class Agent:
    def __init__(self, env):
        self.startEpsilon = 1                           # Pravdepodobnost konani nahodneho tahu na zacatku
        self.endEpsilon = 0.005                         # Pravdepodobnost konani nahodneho tahu na konci
        self.currentEpsilon = self.startEpsilon         # Soucasna pravdepodobnost konani nahodneho tahu
        self.epsilonDiminution = (self.startEpsilon - self.endEpsilon) / 50000#(self.startEpsilon - self.endEpsilon)/(episodes * 2)                  # Hodnota snizovani epsilonu
        self.gamma = 0.99                               # Discount faktor
        self.minibatchSize = 64                         # Velikost minibatche
        self.actionCount = env.action_space.n           # Pocet vstupu do prostredi
        self.stateSize = env.observation_space.shape[0] # Pocet vystupu z prostredi
        self.learningRate = 0.001                       # Learning rate
        self.fractionUpdate = 0.125
        self.memory_size = 10000                        # Velikost Replay memory
        self.memory = deque(maxlen=self.memory_size)
        
        self.net = self.getNN(env)
        self.netTarget = self.getNN(env)
        self.updateTargetNet()

    def getNN(self, env):
        net = Sequential()

        net.add(Dense(32, activation="relu", input_shape=(self.stateSize,), kernel_initializer="he_uniform"))
        net.add(Dense(16, activation="relu", kernel_initializer="he_uniform"))
        net.add(Dense(self.actionCount, activation="linear", kernel_initializer="he_uniform"))

        net.summary()

        net.compile(loss=losses.mean_squared_error, optimizer=optimizers.Adam(lr=self.learningRate), metrics=['accuracy'])

        return net
    
    def updateTargetNet(self):
        self.netTarget.set_weights(self.net.get_weights())
    
    def updateTargetNetPartially(self):
        weights = self.net.get_weights()
        weightsTarget = self.netTarget.get_weights()
        
        for i in range(len(weightsTarget)):
            weightsTarget[i] = weights[i] * self.fractionUpdate + weightsTarget[i] * (1 - self.fractionUpdate)
            
        self.netTarget.set_weights(weightsTarget)
        
    def remember(self, state, action, reward, nextState, done):
        self.memory.append((state, action, reward, nextState, done))
        
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
            return np.random.randint(0, self.actionCount, size=1)[0]
        else:
            Q = self.net.predict(state)
            return np.argmax(Q)

    def get_action(self, state):
        Q = self.net.predict(state)
        return np.argmax(Q)
        
    def resetEpsilon(self):
        self.currentEpsilon = self.startEpsilon
    
    def trainDQN(self):
        if len(self.memory) >= self.minibatchSize:
            minibatch = random.sample(list(self.memory), self.minibatchSize) #z D vybere pocet mb_size samplu
        else:
            return

        state = np.vstack([i[0] for i in minibatch])
        action = [i[1] for i in minibatch]
        reward = [i[2] for i in minibatch]
        nextState = np.vstack([i[3] for i in minibatch])
        done = [i[4] for i in minibatch]

        target_fs = np.zeros((self.minibatchSize, self.actionCount))
        
        for i in range(0, self.minibatchSize):
            if i == 0 : print("State: {}\nState[i]: {}\nnp.array([state[i]]): {}" .format(state,state[i],np.array([state[i]])))
            target_f = self.net.predict(np.array([state[i]]))
            if i == 0 : print("Target_f: {}" .format(target_f))
            if i == 0 : print("Reward[i]: {}" .format(reward[i]))
            if done[i] == 1:
                target_f[0][action[i]] = reward[i]
                if i == 0 : print("Target_f(first): {}" .format(target_f))
            else:
                aNet = self.net.predict(np.array([nextState[i]]))[0]
                if i == 0 : print("aNet: {}" .format(aNet))
                if i == 0 : print("np.max(aNet): {}" .format(np.max(aNet)))
                if i == 0 : print("np.amax(aNet): {}" .format(np.amax(aNet)))
                target_f[0][action[i]] = reward[i] + self.gamma * np.max(aNet)
                if i == 0 : print("Target_f(second): {}" .format(target_f))
            for e in range(self.actionCount):
                target_fs[i][e] = target_f[0][e]
        
        print("State: {}\nTarget_fs: {}".format(state, target_fs))

        self.net.fit(state, target_fs, epochs=1, verbose=0)
                
    def trainDDQN(self):
        if len(self.memory) >= self.minibatchSize:
            minibatch = random.sample(list(self.memory), self.minibatchSize) #z D vybere pocet mb_size samplu
        else:
            return

        state = np.vstack([i[0] for i in minibatch])
        action = [i[1] for i in minibatch]
        reward = [i[2] for i in minibatch]
        nextState = np.vstack([i[3] for i in minibatch])
        done = [i[4] for i in minibatch]

        target_fs = np.zeros((self.minibatchSize, self.actionCount))

        for i in range(0, self.minibatchSize):

            target_f = self.net.predict(np.array([state[i]]))

            if done[i] == 1:
                target_f[0][action[i]] = reward[i]

            else:
                aNet = self.net.predict(np.array([nextState[i]]))[0]
                tNet = self.netTarget.predict(np.array([nextState[i]]))[0]

                target_f[0][action[i]] = reward[i] + self.gamma * tNet[np.argmax(aNet)]
            
            target_fs[i][0] = target_f[0][0]
            target_fs[i][1] = target_f[0][1]

        self.net.fit(state, target_fs, batch_size = self.minibatchSize, verbose = 0, epochs = 32)
    
    def loadNN(self, name):
        self.net.load_weights(name)

    def saveNN(self, name):
        self.net.save_weights(name)