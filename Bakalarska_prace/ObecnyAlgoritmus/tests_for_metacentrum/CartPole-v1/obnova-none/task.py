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
		self.agent = Agent(self.env_state_size, self.env_action_size)
		self.test = self.cp0_test

	def cp0_test(self, episodes, scores, episodes_numbers):
		score = score_estimate(self, 100)
		if score > self.solved_score:
			self.agent.save_model_weights("{}-solved.h5" .format(self.name))
			print("Task solved after {} episodes with score {}." .format(episodes, score))
			scattergrams_graph(scores, episodes_numbers, "learning-cp0-pgraph.png")
			line_graph(scores, episodes_numbers, "learning-cp0-lgraph-i1.png")
			line_graph_i5(scores, episodes_numbers, "learning-cp0-lgraph-i5.png")
			line_graph_i11(scores, episodes_numbers, "learning-cp0-lgraph-i11.png")
			line_graph_i101(scores, episodes_numbers, "learning-cp0-lgraph-i101.png")
			sys.exit()

	def cp1(self, name):
		self.name = name
		self.env = gym.make(name)
		self.env_state_size = self.env.observation_space.shape[0]
		self.env_action_size = self.env.action_space.n
		self.solved_score = 475
		self.agent = Agent(self.env_state_size, self.env_action_size)
		self.test = self.cp1_test

	def cp1_test(self, episodes, scores, episodes_numbers):
		score = score_estimate(self, 100)
		if score > self.solved_score:
			self.agent.save_model_weights("{}-solved.h5" .format(self.name))
			print("Task solved after {} episodes with score {}." .format(episodes, score))
			scattergrams_graph(scores, episodes_numbers, "learning-cp1-pgraph.png")
			line_graph(scores, episodes_numbers, "learning-cp1-lgraph-i1.png")
			line_graph_i5(scores, episodes_numbers, "learning-cp1-lgraph-i5.png")
			line_graph_i11(scores, episodes_numbers, "learning-cp1-lgraph-i11.png")
			line_graph_i101(scores, episodes_numbers, "learning-cp1-lgraph-i101.png")
			sys.exit()
	
	def mt0(self, name):
		self.name = name
		self.env = gym.make(name)
		self.env_state_size = self.env.observation_space.shape[0]
		self.env_action_size = self.env.action_space.n
		self.solved_score = -110
		self.agent = Agent(self.env_state_size, self.env_action_size)
		self.test = self.mt0_test

	def mt0_test(self, episodes, scores, episodes_numbers):
		score = score_estimate(self, 100)
		if score > self.solved_score:
			self.agent.save_model_weights("{}-solved.h5" .format(self.name))
			print("Task solved after {} episodes with score {}." .format(episodes, score))
			scattergrams_graph(scores, episodes_numbers, "learning-mt0-pgraph.png")
			line_graph(scores, episodes_numbers, "learning-mt0-lgraph-i1.png")
			line_graph_i5(scores, episodes_numbers, "learning-mt0-lgraph-i5.png")
			line_graph_i11(scores, episodes_numbers, "learning-mt0-lgraph-i11.png")
			line_graph_i101(scores, episodes_numbers, "learning-mt0-lgraph-i101.png")
			sys.exit()

	def a1(self, name):
		self.name = name
		self.env = gym.make(name)
		self.env_state_size = self.env.observation_space.shape[0]
		self.env_action_size = self.env.action_space.n
		self.solved_score = float("-inf")
		self.agent = Agent(self.env_state_size, self.env_action_size)
		self.test = self.a1_test

	def a1_test(self, episodes, scores, episodes_numbers):
		if episodes == 99:
			score = score_estimate(self, 100)
			self.agent.save_model_weights("{}-solved.h5" .format(self.name))
			print("Task solved after {} episodes with score {}." .format(episodes, score))
			scattergrams_graph(scores, episodes_numbers, "learning-a1-pgraph.png")
			line_graph(scores, episodes_numbers, "learning-a1-lgraph-i1.png")
			line_graph_i5(scores, episodes_numbers, "learning-a1-lgraph-i5.png")
			line_graph_i11(scores, episodes_numbers, "learning-a1-lgraph-i11.png")
			line_graph_i101(scores, episodes_numbers, "learning-a1-lgraph-i101.png")
			sys.exit()