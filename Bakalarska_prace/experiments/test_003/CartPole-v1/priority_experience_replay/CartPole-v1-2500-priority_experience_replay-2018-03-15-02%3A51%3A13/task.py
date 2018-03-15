import gym
import h5py
import sys
from agent import *
from playing import *
from visualization import *

class Task:
    def __init__(self, name):
        self.envs = {"CartPole-v0" : self.cp0,
                    "CartPole-v1" : self.cp1,
                    "MountainCar-v0" : self.mt0,
                    "Acrobot-v1" : self.a1,
                    }
        self.envs[name](name)

    def cp0(self, name):
        self.name = name
        self.env = gym.make(name)
        self.env_state_size = self.env.observation_space.shape[0]
        self.env_action_size = self.env.action_space.n
        self.solved_score = 195
        self.max_steps = 200
        self.agent = Agent(self.env_state_size, self.env_action_size)
        self.test = self.cp0_test

    def cp0_test(self, episodes, scores, episodes_numbers):
        first_estimation = score_estimate(self, 10)
        if first_estimation < self.solved_score:
            return

        second_estimation = score_estimate(self, 90)

        score = 0.1 * first_estimation + (0.9 * second_estimation)

        if score > self.solved_score:
            self.agent.save_model_weights("{}-solved.h5" .format(self.name))
            print("Task solved after {} episodes with score {}." .format(episodes, score))
            point_graph(scores, episodes_numbers, "{}-point_graph.png" .format(self.name))
            gaussian_graph(scores, episodes_numbers, "{}-gaussian_graph.png" .format(self.name))
            combined_graph(scores, episodes_numbers, "{}-combined_graph.png" .format(self.name))
            sys.exit()

    def cp1(self, name):
        self.name = name
        self.env = gym.make(name)
        self.env_state_size = self.env.observation_space.shape[0]
        self.env_action_size = self.env.action_space.n
        self.solved_score = 475
        self.max_steps = 500
        self.agent = Agent(self.env_state_size, self.env_action_size)
        self.test = self.cp1_test

    def cp1_test(self, episodes, scores, episodes_numbers):
        first_estimation = score_estimate(self, 10)
        if first_estimation < self.solved_score:
            return

        second_estimation = score_estimate(self, 90)

        score = 0.1 * first_estimation + (0.9 * second_estimation)

        if score > self.solved_score:
            self.agent.save_model_weights("{}-solved.h5" .format(self.name))
            print("Task solved after {} episodes with score {}." .format(episodes, score))
            point_graph(scores, episodes_numbers, "{}-point_graph.png" .format(self.name))
            gaussian_graph(scores, episodes_numbers, "{}-gaussian_graph.png" .format(self.name))
            combined_graph(scores, episodes_numbers, "{}-combined_graph.png" .format(self.name))
            sys.exit()
    
    def mt0(self, name):
        self.name = name
        self.env = gym.make(name)
        self.env_state_size = self.env.observation_space.shape[0]
        self.env_action_size = self.env.action_space.n
        self.solved_score = -110
        self.max_steps = 200
        self.agent = Agent(self.env_state_size, self.env_action_size)
        self.test = self.mt0_test

    def mt0_test(self, episodes, scores, episodes_numbers):
        first_estimation = score_estimate(self, 10)
        if first_estimation < self.solved_score:
            return

        second_estimation = score_estimate(self, 90)

        score = 0.1 * first_estimation + (0.9 * second_estimation)
        
        if score > self.solved_score:
            self.agent.save_model_weights("{}-solved.h5" .format(self.name))
            print("Task solved after {} episodes with score {}." .format(episodes, score))
            point_graph(scores, episodes_numbers, "{}-point_graph.png" .format(self.name))
            gaussian_graph(scores, episodes_numbers, "{}-gaussian_graph.png" .format(self.name))
            combined_graph(scores, episodes_numbers, "{}-combined_graph.png" .format(self.name))
            sys.exit()

    def a1(self, name):
        self.name = name
        self.env = gym.make(name)
        self.env_state_size = self.env.observation_space.shape[0]
        self.env_action_size = self.env.action_space.n
        self.solved_score = float("-inf")
        self.max_steps = 500
        self.agent = Agent(self.env_state_size, self.env_action_size)
        self.test = self.a1_test

    def a1_test(self, episodes, scores, episodes_numbers):
        if episodes == 99:
            score = score_estimate(self, 100)
            self.agent.save_model_weights("{}-solved.h5" .format(self.name))
            print("Task solved after {} episodes with score {}." .format(episodes, score))
            point_graph(scores, episodes_numbers, "{}-point_graph.png" .format(self.name))
            gaussian_graph(scores, episodes_numbers, "{}-gaussian_graph.png" .format(self.name))
            combined_graph(scores, episodes_numbers, "{}-combined_graph.png" .format(self.name))
            sys.exit()