"""
docstring
"""
import numpy as np
from keras.models import Sequential, Model
from keras.layers import Dense, Input, Concatenate, Subtract, Lambda, Add
from keras import backend as K
from keras import optimizers, losses
from memory import Memory


class Agent:
    """
    docstring
    """
    def __init__(self, state_size, action_size):
        self.initial_epsilon = 1
        self.final_epsilon = 0.005
        self.current_epsilon = self.initial_epsilon
        self.epsilon_decay = 0.00002
        self.gamma = 0.99
        self.minibatch_size = 256
        self.learning_rate = 0.001
        self.fraction_update = 0.125
        self.memory_size = 10000
        self.memory = Memory(self.memory_size)
        self.action_size = action_size
        self.state_size = state_size

        self.model_net = self.build_model_dueling()
        self.target_net = self.build_model_dueling()
        self.update_target_net()

    def make_model(self):
        """
        docstring
        """
        model_net = Sequential()

        model_net.add(Dense(32, activation="relu", input_shape=(self.state_size,), kernel_initializer="he_uniform"))
        model_net.add(Dense(16, activation="relu", kernel_initializer="he_uniform"))
        model_net.add(Dense(self.action_size, activation="linear", kernel_initializer="he_uniform"))

        model_net.summary()

        model_net.compile(loss=losses.mean_squared_error, optimizer=optimizers.RMSprop(lr=self.learning_rate), metrics=['accuracy'])

        return model_net

    def build_model_dueling(self):
        inp=Input(shape=(self.state_size,))
        x=Dense(units=32,activation='relu',kernel_initializer='he_uniform')(inp)
        x=Dense(units=16,activation='relu',kernel_initializer='he_uniform')(x)
        value_=Dense(units=1,activation='linear',kernel_initializer='he_uniform')(x)
        ac_activation=Dense(units=self.action_size,activation='linear',kernel_initializer='he_uniform')(x)

        avg_ac_activation=Lambda(lambda x: K.mean(x,axis=1,keepdims=True))(ac_activation)

        concat_value=Concatenate(axis=-1)([value_,value_])
        concat_avg_ac=Concatenate(axis=-1)([avg_ac_activation,avg_ac_activation])
        
        for i in range(1,self.action_size-1):
            concat_value=Concatenate(axis=-1)([concat_value,value_])
            concat_avg_ac=Concatenate(axis=-1)([concat_avg_ac,avg_ac_activation])
        
        ac_activation=Subtract()([ac_activation,concat_avg_ac])

        merged_layers=Add()([concat_value,ac_activation])
        final_model=Model(inputs=inp,outputs=merged_layers)
        final_model.summary()
        final_model.compile(loss='mean_squared_error',optimizer=optimizers.Adam(lr=self.learning_rate))
        return final_model

    def update_target_net(self):
        """
        docstring
        """
        self.target_net.set_weights(self.model_net.get_weights())

    def update_target_net_partially(self):
        """
        docstring
        """
        weights_model = self.model_net.get_weights()
        weights_target = self.target_net.get_weights()

        for i in range(len(weights_target)):
            weights_target[i] = weights_model[i] * self.fraction_update + weights_target[i] * (1 - self.fraction_update)

        self.target_net.set_weights(weights_target)

    def get_error(self, state, action, reward, next_state, done):
        """
        docstring
        """
        q_value = self.model_net.predict(state)
        ns_model_pred = self.model_net.predict(np.array([next_state]))
        ns_target_pred = self.target_net.predict(np.array([next_state]))

        obs_error = q_value[0][action]

        if done == 1:
            q_value[0][action] = reward
        else:
            q_value[0][action] = reward + self.gamma * ns_target_pred[0][np.argmax(ns_model_pred)]

        obs_error = abs(obs_error - q_value[0][action])

        return obs_error

    def remember(self, state, action, reward, next_state, done, rand_agent):
        """
        docstring
        """
        if rand_agent:
            obs_error = abs(reward)
        else:
            obs_error = self.get_error(state, action, reward, next_state, done)

        self.memory.add_observation((state, action, reward, next_state, done), obs_error)

    def clear_memory(self):
        """
        docstring
        """
        self.memory.clear()

    def decrease_epsilon(self):
        """
        docstring
        """
        if self.current_epsilon > self.final_epsilon:
            if (self.current_epsilon - self.epsilon_decay) > self.final_epsilon:
                self.current_epsilon = self.current_epsilon - self.epsilon_decay
            else:
                self.current_epsilon = self.final_epsilon

    def get_action(self, state, epsilon):
        """
        docstring
        """
        if not epsilon:
            q_value = self.model_net.predict(state)
            return np.argmax(q_value)

        if np.random.rand() <= self.current_epsilon:
            return np.random.randint(0, self.action_size, size=1)[0]
        else:
            q_value = self.model_net.predict(state)
            return np.argmax(q_value)

    def train_dqn(self):
        """
        docstring
        """
        if self.memory.length >= self.minibatch_size:
            minibatch = self.memory.sample(self.minibatch_size)
        else:
            return

        state = np.vstack([i[1][0] for i in minibatch])
        action = [i[1][1] for i in minibatch]
        reward = [i[1][2] for i in minibatch]
        next_state = np.vstack([i[1][3] for i in minibatch])
        done = [i[1][4] for i in minibatch]

        errors = np.zeros(self.minibatch_size)

        q_value = self.model_net.predict(state)
        ns_model_pred = self.model_net.predict(next_state)

        for i in range(0, self.minibatch_size):
            errors[i] = q_value[i][action[i]]

            if done[i] == 1:
                q_value[i][action[i]] = reward[i]
            else:
                q_value[i][action[i]] = reward[i] + self.gamma * np.max(ns_model_pred[i])

            errors[i] = abs(errors[i] - q_value[i][action[i]])

        self.model_net.fit(state, q_value, epochs=1, verbose=0)
        self.memory.update_minibatch(minibatch, errors)

    def train_target_dqn(self):
        """
        docstring
        """
        if self.memory.length >= self.minibatch_size:
            minibatch = self.memory.sample(self.minibatch_size)
        else:
            return

        state = np.vstack([i[1][0] for i in minibatch])
        action = [i[1][1] for i in minibatch]
        reward = [i[1][2] for i in minibatch]
        next_state = np.vstack([i[1][3] for i in minibatch])
        done = [i[1][4] for i in minibatch]

        errors = np.zeros(self.minibatch_size)

        q_value = self.model_net.predict(state)
        ns_target_pred = self.target_net.predict(next_state)

        for i in range(0, self.minibatch_size):
            errors[i] = q_value[i][action[i]]

            if done[i] == 1:
                q_value[i][action[i]] = reward[i]
            else:
                q_value[i][action[i]] = reward[i] + self.gamma * np.max(ns_target_pred[i])

            errors[i] = abs(errors[i] - q_value[i][action[i]])

        self.model_net.fit(state, q_value, epochs=1, verbose=0)
        self.memory.update_minibatch(minibatch, errors)

    def train_ddqn(self):
        """
        docstring
        """
        if self.memory.length >= self.minibatch_size:
            minibatch = self.memory.sample(self.minibatch_size)
        else:
            return

        state = np.vstack([i[1][0] for i in minibatch])
        action = [i[1][1] for i in minibatch]
        reward = [i[1][2] for i in minibatch]
        next_state = np.vstack([i[1][3] for i in minibatch])
        done = [i[1][4] for i in minibatch]

        errors = np.zeros(self.minibatch_size)

        q_value = self.model_net.predict(state)
        ns_model_pred = self.model_net.predict(next_state)
        ns_target_pred = self.target_net.predict(next_state)

        for i in range(0, self.minibatch_size):
            errors[i] = q_value[i][action[i]]

            if done[i] == 1:
                q_value[i][action[i]] = reward[i]
            else:
                q_value[i][action[i]] = reward[i] + self.gamma * ns_target_pred[i][np.argmax(ns_model_pred[i])]

            errors[i] = abs(errors[i] - q_value[i][action[i]])

        self.model_net.fit(state, q_value, epochs=1, verbose=0)
        #self.memory.update_minibatch(minibatch, errors)

    def load_model_weights(self, name):
        """
        docstring
        """
        self.model_net.load_weights(name)

    def save_model_weights(self, name):
        """
        docstring
        """
        self.model_net.save_weights("./model-{}".format(name))

    def load_target_weights(self, name):
        """
        docstring
        """
        self.target_net.load_weights(name)

    def save_target_weights(self, name):
        """
        docstring
        """
        self.target_net.save_weights("./target-{}".format(name))
