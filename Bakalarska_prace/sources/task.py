"""
docstring
"""
import sys
import gym
import gym_2048
from gym import wrappers
from agent import Agent
from playing import Playing as pl
from visualization import combined_graph

class Task:
    """
    docstring
    """
    def __init__(self, args):
        self.args = args
        self.name = None
        self.env = None
        self.env_state_size = None
        self.env_action_size = None
        self.type = None
        self.solved_score = None
        self.average_rand_score = None
        self.max_steps = None
        self.agent = None
        self.test = None
        self.envs = {"CartPole-v0" : self.cartp0,
                     "CartPole-v1" : self.cartp1,
                     "MountainCar-v0" : self.mcar0,
                     "Acrobot-v1" : self.acro1,
                     "2048-v0" : self.tfe0,
                     "Breakout-v0" : self.break0,
                     "SpaceInvaders-v0" : self.space0,
                     "BeamRider-v0" : self.beam0,
                     "Breakout-ram-v0" : self.breakR0,
                     "SpaceInvaders-ram-v0" : self.spaceR0,
                     "BeamRider-ram-v0" : self.beamR0,
                    }
        self.envs[args.environment]()

    def cartp0(self):
        """
        docstring
        """
        self.name = self.args.environment
        self.env = gym.make(self.name)
        self.env_state_size = self.env.observation_space.shape[0]
        self.env_action_size = self.env.action_space.n
        self.type = "basic"
        self.solved_score = 195
        self.average_rand_score = 22.25
        self.max_steps = 200
        self.agent = Agent(self.args.algorithm, self.env_state_size, self.env_action_size,
                           self.args.network, self.args.memory, [32,16])
        self.test = self.cartp0_test

    def cartp0_test(self, scores, episodes_numbers):
        """
        docstring
        """
        complete_estimation = pl.score_estimate_fs(self, 10)
        if complete_estimation >= self.solved_score:
            for i in range(2, 11):
                estimation = pl.score_estimate_fs(self, 10)
                complete_estimation = complete_estimation + estimation
                if (complete_estimation / i) < self.solved_score:
                    return
        else:
            return
        score = complete_estimation / 10

        if score > self.solved_score:
            self.agent.save_model_weights("{}-solved.h5" .format(self.name))
            print("[Model was saved.]")
            combined_graph(scores, episodes_numbers, "{}_results.pdf" .format(self.name), [episodes_numbers[-1]+10,max(scores)+10], self.average_rand_score)
            print("[Graph of learning progress visualization was made.]")
            print("[Task was solved after {} episodes with score {}.]" .format(episodes_numbers[-1], score))
            sys.exit()

    def cartp1(self):
        """
        docstring
        """
        self.name = self.args.environment
        self.env = gym.make(self.name)
        self.env_state_size = self.env.observation_space.shape[0]
        self.env_action_size = self.env.action_space.n
        self.type = "basic"
        self.solved_score = 475
        self.average_rand_score = 22.25
        self.max_steps = 500
        self.agent = Agent(self.args.algorithm, self.env_state_size, self.env_action_size,
                           self.args.network, self.args.memory, [32,16])
        self.test = self.cartp1_test

    def cartp1_test(self, scores, episodes_numbers):
        """
        docstring
        """
        complete_estimation = pl.score_estimate_fs(self, 10)
        if complete_estimation >= self.solved_score:
            for i in range(2, 11):
                estimation = pl.score_estimate_fs(self, 10)
                complete_estimation = complete_estimation + estimation
                if (complete_estimation / i) < self.solved_score:
                    return
        else:
            return
        score = complete_estimation / 10

        if score > self.solved_score:
            self.agent.save_model_weights("{}-solved.h5" .format(self.name))
            print("[Model was saved.]")
            combined_graph(scores, episodes_numbers, "{}_results.pdf" .format(self.name), [episodes_numbers[-1]+10,max(scores)+10], self.average_rand_score)
            print("[Graph of learning progress visualization was made.]")
            print("[Task was solved after {} episodes with score {}.]" .format(episodes_numbers[-1], score))
            sys.exit()

    def mcar0(self):
        """
        docstring
        """
        self.name = self.args.environment
        self.env = gym.make(self.name)
        self.env_state_size = self.env.observation_space.shape[0]
        self.env_action_size = self.env.action_space.n
        self.type = "basic"
        self.solved_score = -110
        self.average_rand_score = -200
        self.max_steps = 200
        self.agent = Agent(self.args.algorithm, self.env_state_size, self.env_action_size,
                           self.args.network, self.args.memory, [32,16])
        self.test = self.mcar0_test

    def mcar0_test(self, scores, episodes_numbers):
        """
        docstring
        """
        complete_estimation = pl.score_estimate_fs(self, 10)
        if complete_estimation >= self.solved_score:
            for i in range(2, 11):
                estimation = pl.score_estimate_fs(self, 10)
                complete_estimation = complete_estimation + estimation
                if (complete_estimation / i) < self.solved_score:
                    return
        else:
            return
        score = complete_estimation / 10

        if score > self.solved_score:
            self.agent.save_model_weights("{}-solved.h5" .format(self.name))
            print("[Model was saved.]")
            combined_graph(scores, episodes_numbers, "{}_results.pdf" .format(self.name), [episodes_numbers[-1]+10,max(scores)+10], self.average_rand_score)
            print("[Graph of learning progress visualization was made.]")
            print("[Task was solved after {} episodes with score {}.]" .format(episodes_numbers[-1], score))
            sys.exit()

    def acro1(self):
        """
        docstring
        """
        self.name = self.args.environment
        self.env = gym.make(self.name)
        self.env_state_size = self.env.observation_space.shape[0]
        self.env_action_size = self.env.action_space.n
        self.type = "basic"
        self.solved_score = None
        self.average_rand_score = -498.95
        self.max_steps = 500
        self.agent = Agent(self.args.algorithm, self.env_state_size, self.env_action_size,
                           self.args.network, self.args.memory, [32,16])
        self.test = self.acro1_test

    def acro1_test(self, scores, episodes_numbers):
        """
        docstring
        """
        if episodes_numbers[-1] == 99:
            score = pl.score_estimate_fs(self, 100)
            self.agent.save_model_weights("{}-solved.h5" .format(self.name))
            print("[Model was saved.]")
            combined_graph(scores, episodes_numbers, "{}_results.pdf" .format(self.name), [episodes_numbers[-1]+10,max(scores)+10], self.average_rand_score)
            print("[Graph of learning progress visualization was made.]")
            print("[Task was solved after {} episodes with score {}.]" .format(episodes_numbers[-1], score))
            sys.exit()

    def tfe0(self):
        """
        docstring
        """
        self.name = self.args.environment
        self.env = gym.make(self.name)
        self.env_state_size = self.env.observation_space.shape[0]
        self.env_action_size = self.env.action_space.n
        self.type = "text"
        self.solved_score = None
        self.average_rand_score = 1011
        self.max_steps = 100000
        self.agent = Agent(self.args.algorithm, self.env_state_size, self.env_action_size,
                           self.args.network, self.args.memory, [256, 256])
        self.test = None

    def break0(self):
        """
        docstring
        """
        self.name = self.args.environment
        self.env = gym.make(self.name)
        self.env_state_size = (2, 84, 84)
        self.env_action_size = self.env.action_space.n
        self.type = "image"
        self.solved_score = None
        self.average_rand_score = 1.2
        self.max_steps = 10000
        self.agent = Agent(self.args.algorithm, self.env_state_size, self.env_action_size,
                           self.args.network, self.args.memory)
        self.test = None

    def space0(self):
        """
        docstring
        """
        self.name = self.args.environment
        self.env = gym.make(self.name)
        self.env_state_size = (2, 84, 84)
        self.env_action_size = self.env.action_space.n
        self.type = "image"
        self.solved_score = None
        self.average_rand_score = 179
        self.max_steps = 10000
        self.agent = Agent(self.args.algorithm, self.env_state_size, self.env_action_size,
                           self.args.network, self.args.memory)
        self.test = None

    def beam0(self):
        """
        docstring
        """
        self.name = self.args.environment
        self.env = gym.make(self.name)
        self.env_state_size = (2, 84, 84)
        self.env_action_size = self.env.action_space.n
        self.type = "image"
        self.solved_score = None
        self.average_rand_score = 354
        self.max_steps = 10000
        self.agent = Agent(self.args.algorithm, self.env_state_size, self.env_action_size,
                           self.args.network, self.args.memory)
        self.test = None

    def breakR0(self):
        """
        docstring
        """
        self.name = self.args.environment
        self.env = gym.make(self.name)
        self.env_state_size = (128)
        self.env_action_size = self.env.action_space.n
        self.type = "ram"
        self.solved_score = None
        self.average_rand_score = 1.2
        self.max_steps = 10000
        self.agent = Agent(self.args.algorithm, self.env_state_size, self.env_action_size,
                           self.args.network, self.args.memory, 512)
        self.test = None

    def spaceR0(self):
        """
        docstring
        """
        self.name = self.args.environment
        self.env = gym.make(self.name)
        self.env_state_size = (128)
        self.env_action_size = self.env.action_space.n
        self.type = "ram"
        self.solved_score = None
        self.average_rand_score = 179
        self.max_steps = 10000
        self.agent = Agent(self.args.algorithm, self.env_state_size, self.env_action_size,
                           self.args.network, self.args.memory, 512)
        self.test = None

    def beamR0(self):
        """
        docstring
        """
        self.name = self.args.environment
        self.env = gym.make(self.name)
        self.env_state_size = (128)
        self.env_action_size = self.env.action_space.n
        self.type = "ram"
        self.solved_score = None
        self.average_rand_score = 354
        self.max_steps = 10000
        self.agent = Agent(self.args.algorithm, self.env_state_size, self.env_action_size,
                           self.args.network, self.args.memory, 512)
        self.test = None
