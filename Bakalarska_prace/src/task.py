"""
file contains class Task, which is wrapper
for working with different environments
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
    class for working with different environments
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
        initialization method for working with environment CartPole-v0
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

    def cartp0_test(self, scores, episode_numbers):
        """
        method tests CartPole-v0 solution
        """
        complete_estimation = pl.agent_score_estimate(self, 10)
        if complete_estimation >= self.solved_score:
            for i in range(2, 11):
                estimation = pl.agent_score_estimate(self, 10)
                complete_estimation = complete_estimation + estimation
                if (complete_estimation / i) < self.solved_score:
                    return
        else:
            return
        score = complete_estimation / 10

        if score > self.solved_score:
            self.agent.save_model_weights("{}-solved.h5" .format(self.name))
            print("[Model was saved.]")
            combined_graph(scores, episode_numbers, "{}_results" .format(self.name), 
                           [episode_numbers[-1], max(scores)+10], {self.average_rand_score:self.average_rand_score}, scatter=True)
            print("[Graph of learning progress visualization was made.]")
            print("[Task was solved after {} episodes with score {}.]" .format(episode_numbers[-1], score))
            sys.exit()

    def cartp1(self):
        """
        initialization method for working with environment CartPole-v1
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

    def cartp1_test(self, scores, episode_numbers):
        """
        method tests CartPole-v1 solution
        """
        complete_estimation = pl.agent_score_estimate(self, 10)
        if complete_estimation >= self.solved_score:
            for i in range(2, 11):
                estimation = pl.agent_score_estimate(self, 10)
                complete_estimation = complete_estimation + estimation
                if (complete_estimation / i) < self.solved_score:
                    return
        else:
            return
        score = complete_estimation / 10

        if score > self.solved_score:
            self.agent.save_model_weights("{}-solved.h5" .format(self.name))
            print("[Model was saved.]")
            combined_graph(scores, episode_numbers, "{}_results" .format(self.name), 
                           [episode_numbers[-1], max(scores)+10], {self.average_rand_score:self.average_rand_score}, scatter=True)
            print("[Graph of learning progress visualization was made.]")
            print("[Task was solved after {} episodes with score {}.]" .format(episode_numbers[-1], score))
            sys.exit()

    def mcar0(self):
        """
        initialization method for working with environment MountainCar-v0
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

    def mcar0_test(self, scores, episode_numbers):
        """
        method tests MountainCar-v0 solution
        """
        complete_estimation = pl.agent_score_estimate(self, 10)
        if complete_estimation >= self.solved_score:
            for i in range(2, 11):
                estimation = pl.agent_score_estimate(self, 10)
                complete_estimation = complete_estimation + estimation
                if (complete_estimation / i) < self.solved_score:
                    return
        else:
            return
        score = complete_estimation / 10

        if score > self.solved_score:
            self.agent.save_model_weights("{}-solved.h5" .format(self.name))
            print("[Model was saved.]")
            combined_graph(scores, episode_numbers, "{}_results" .format(self.name), 
                           [episode_numbers[-1], max(scores)+10], {self.average_rand_score:self.average_rand_score}, scatter=True)
            print("[Graph of learning progress visualization was made.]")
            print("[Task was solved after {} episodes with score {}.]" .format(episode_numbers[-1], score))
            sys.exit()

    def acro1(self):
        """
        initialization method for working with environment Acrobot-v1
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

    def acro1_test(self, scores, episode_numbers):
        """
        method tests Acrobot-v1 solution
        """
        if episode_numbers[-1] == 99:
            score = pl.agent_score_estimate(self, 100)
            self.agent.save_model_weights("{}-solved.h5" .format(self.name))
            print("[Model was saved.]")
            combined_graph(scores, episode_numbers, "{}_results" .format(self.name), 
                           [episode_numbers[-1], max(scores)+10], {self.average_rand_score:self.average_rand_score}, scatter=True)
            print("[Graph of learning progress visualization was made.]")
            print("[Task was solved after {} episodes with score {}.]" .format(episode_numbers[-1], score))
            sys.exit()

    def tfe0(self):
        """
        initialization method for working with environment 2048-v0
        """
        self.name = self.args.environment
        self.env = gym.make(self.name)
        self.env_state_size = self.env.observation_space.shape[0]
        self.env_action_size = self.env.action_space.n
        self.type = "text"
        self.solved_score = None
        self.average_rand_score = 1011
        self.max_steps = 1000
        self.agent = Agent(self.args.algorithm, self.env_state_size, self.env_action_size,
                           self.args.network, self.args.memory, [256, 256])
        self.test = None

    def break0(self):
        """
        initialization method for working with environment Breakout-v0
        """
        self.name = self.args.environment
        self.env = gym.make(self.name)
        self.env_state_size = (self.args.num_of_frames, 84, 84)
        self.env_action_size = self.env.action_space.n
        self.type = "image"
        self.solved_score = 31
        self.average_rand_score = 1.2
        self.max_steps = 10000
        self.agent = Agent(self.args.algorithm, self.env_state_size, self.env_action_size,
                           self.args.network, self.args.memory)
        self.test = None

    def space0(self):
        """
        initialization method for working with environment SpaceInvaders-v0
        """
        self.name = self.args.environment
        self.env = gym.make(self.name)
        self.env_state_size = (self.args.num_of_frames, 84, 84)
        self.env_action_size = self.env.action_space.n
        self.type = "image"
        self.solved_score = 3690
        self.average_rand_score = 179
        self.max_steps = 10000
        self.agent = Agent(self.args.algorithm, self.env_state_size, self.env_action_size,
                           self.args.network, self.args.memory)
        self.test = None

    def beam0(self):
        """
        initialization method for working with environment BeamRider-v0
        """
        self.name = self.args.environment
        self.env = gym.make(self.name)
        self.env_state_size = (self.args.num_of_frames, 84, 84)
        self.env_action_size = self.env.action_space.n
        self.type = "image"
        self.solved_score = 7456
        self.average_rand_score = 354
        self.max_steps = 10000
        self.agent = Agent(self.args.algorithm, self.env_state_size, self.env_action_size,
                           self.args.network, self.args.memory)
        self.test = None

    def breakR0(self):
        """
        initialization method for working with environment Breakout-ram-v0
        """
        self.name = self.args.environment
        self.env = gym.make(self.name)
        self.env_state_size = (128)
        self.env_action_size = self.env.action_space.n
        self.type = "ram"
        self.solved_score = 31
        self.average_rand_score = 1.2
        self.max_steps = 10000
        self.agent = Agent(self.args.algorithm, self.env_state_size, self.env_action_size,
                           self.args.network, self.args.memory, 512)
        self.test = None

    def spaceR0(self):
        """
        initialization method for working with environment SpaceInvaders-ram-v0
        """
        self.name = self.args.environment
        self.env = gym.make(self.name)
        self.env_state_size = (128)
        self.env_action_size = self.env.action_space.n
        self.type = "ram"
        self.solved_score = 3690
        self.average_rand_score = 179
        self.max_steps = 10000
        self.agent = Agent(self.args.algorithm, self.env_state_size, self.env_action_size,
                           self.args.network, self.args.memory, 512)
        self.test = None

    def beamR0(self):
        """
        initialization method for working with environment BeamRider-ram-v0
        """
        self.name = self.args.environment
        self.env = gym.make(self.name)
        self.env_state_size = (128)
        self.env_action_size = self.env.action_space.n
        self.type = "ram"
        self.solved_score = 7456
        self.average_rand_score = 354
        self.max_steps = 10000
        self.agent = Agent(self.args.algorithm, self.env_state_size, self.env_action_size,
                           self.args.network, self.args.memory, 512)
        self.test = None
