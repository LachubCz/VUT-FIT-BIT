import numpy as np
import random

class Memory:
	capacity = None
	priority_tree = None
	observation_tree = None
	nw_elem_idx = None
	basic_error = None
	error_power = None
	length = None

	def __init__(self, capacity):
		self.capacity = capacity
		self.priority_tree = np.zeros(2*self.capacity - 1)
		self.observation_tree = np.zeros(self.capacity, dtype=object)
		self.nw_elem_idx = 0
		self.basic_error = 0.01
		self.error_power = 0
		self.length = 0

	def sample(self, minibatch_size):
		minibatch = []
		section = self.priority_tree[0] / minibatch_size

		for i in range(minibatch_size):
			a = section * i
			b = section * (i + 1)
			p = random.uniform(a, b)
			minibatch.append(self.get_observation(0, p))

		return minibatch

	def get_observation(self, index, p):
		left = 2 * index + 1

		if left >= len(self.priority_tree):
			return (index, self.observation_tree[index - self.capacity + 1])
        
		if p <= self.priority_tree[left]:
			return self.get_observation(left, p)
		else:
			right = 2 * index + 2
			return self.get_observation(right, p - self.priority_tree[left])

	def get_priority(self, obs_error):
		priority = (obs_error + self.basic_error) ** self.error_power
		return priority

	def add_observation(self, observation, obs_error):
		self.observation_tree[self.nw_elem_idx] = observation
		
		priority = self.get_priority(obs_error)
		index = self.nw_elem_idx + self.capacity - 1
		
		difference = priority - self.priority_tree[index]
		self.priority_tree[index] = priority

		self.propagate_changes(index, difference)

		if self.length != self.capacity:
			self.length = self.length + 1
		
		self.nw_elem_idx = self.nw_elem_idx + 1
		if self.nw_elem_idx >= self.capacity:
			self.nw_elem_idx = 0

	def propagate_changes(self, index, difference):
		parent = (index - 1) // 2

		self.priority_tree[parent] += difference

		if parent != 0:
			self.propagate_changes(parent, difference)

	def update_minibatch(self, minibatch, obs_errors):
		for i in range(len(minibatch)):
			index = minibatch[i][0]

			priority = self.get_priority(obs_errors[i])

			difference = priority - self.priority_tree[index]
			self.priority_tree[index] = priority

			self.propagate_changes(index, difference)

	def clear(self):
		self.priority_tree = np.zeros(2*self.capacity - 1)
		self.observation_tree = np.zeros(self.capacity, dtype=object)
		self.nw_elem_idx = 0