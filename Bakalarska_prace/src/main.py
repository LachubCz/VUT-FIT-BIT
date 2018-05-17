#!/usr/bin/env python
"""
file contains main
"""
import os
import sys
import os.path
import argparse
import scipy
import warnings
warnings.simplefilter('ignore', FutureWarning)
import numpy as np
import tensorflow as tf
from keras.backend.tensorflow_backend import set_session
from keras import backend as K
import gym.wrappers as wrappers

from task import Task
from visualization import combined_graph
from playing import Playing as pl
from playing import engineer_img
from agent import Agent

def err_print(*args, **kwargs):
    """
    method for printing to stderr
    """
    print(*args, file=sys.stderr, **kwargs)
    
def get_args():
    """
    method for parsing of arguments
    """
    parser = argparse.ArgumentParser()

    parser.add_argument("-init", action="store_true", default=False, 
                        help="init replay memory with random agent observations")
    parser.add_argument("-mem", action="store", dest="memory", choices=["basic", "prioritized"], 
                        default="basic", help="type of memory")
    parser.add_argument("-net", action="store", dest="network", choices=["basic", "dueling"], 
                        default="basic", help="type of neural network architecture")
    parser.add_argument("-alg", action="store", dest="algorithm", choices=["DQN", "DQN+TN", "DDQN"], 
                        default="DQN", help="type of algorithm")
    parser.add_argument("-env", action="store", dest="environment", required=True,
                        choices=["CartPole-v0", "CartPole-v1", "MountainCar-v0", "Acrobot-v1", "2048-v0",
                                 "Breakout-v0", "SpaceInvaders-v0", "BeamRider-v0", "Breakout-ram-v0",
                                 "SpaceInvaders-ram-v0", "BeamRider-ram-v0"],
                        help="name of game environment")
    parser.add_argument("-eps", action="store", dest="episodes", type=int, required=True,
                        help="number of training episodes")
    parser.add_argument("-mdl", action="store", dest="model", help="name of file which contains already trained model to load")
    parser.add_argument("-pu", action="store", dest="process_unit", choices=["CPU", "GPU"], default="CPU", 
                        help="script will run on this processing unit")
    parser.add_argument("-vids", action="store_true", default=False,
                        help="script will record videos using gym.wrappers")
    parser.add_argument("-update_f", action="store", dest="update_f", type=int, default=10000,
                        help="frequency of updating target net")
    parser.add_argument("-save_f", action="store", dest="save_f", type=int, default=25,
                        help="frequency of saving model")
    parser.add_argument("-num_of_frames", action="store", dest="num_of_frames", type=int, default=1, choices=[1, 2, 3, 4],
                        help="Number of frames to process as a information of state.")
    parser.add_argument("-mode", action="store", dest="mode", required=True,
                        choices=["train", "test", "render"], default="train", 
                        help="application mode")

    args = parser.parse_args()

    basic = ["CartPole-v0", "CartPole-v1", "MountainCar-v0", "Acrobot-v1", "2048-v0"]
    image = ["Breakout-v0", "SpaceInvaders-v0", "BeamRider-v0"]
    ram = ["Breakout-ram-v0", "SpaceInvaders-ram-v0", "BeamRider-ram-v0"]

    if args.network == "basic":
        if args.environment in basic:
            args.network = "2layer_bsc_mdl"
        if args.environment in image:
            args.network = "bsc_img_mdl"
        if args.environment in ram:
            args.network = "1layer_ram_mdl"
    if args.network == "dueling":
        if args.environment in basic:
            args.network = "2layer_duel_mdl"
        if args.environment in image:
            args.network = "duel_img_model"
        if args.environment in ram:
            err_print("There is no dueling model for this environment.")
            sys.exit(-1)

    if args.model is not None:
        if not os.path.isfile(args.model):
            err_print("Model file doesn't exist.")
            sys.exit(-1)

    if args.num_of_frames != 1 and args.environment not in image:
        err_print("Application doesn't support buffer of states for this environment.")
        sys.exit(-1)
    elif args.num_of_frames == 1 and args.environment in image:
        err_print("Application needs buffer of states for this environment.")
        sys.exit(-1)

    return args

def train(task, normalize_score=True):
    """
    method training
    """
    episodes_numbers = []
    highest = []
    true_scores = []
    normalized_scores = []

    steps = 0

    for eps in range(task.args.episodes):
        state = task.env.reset()
        last_state = state

        if task.type == "text": 
            state = state / 16384
        elif task.type == "ram":
            state = state / 255
        elif task.type == "image":
            state = engineer_img(state)

        if task.args.num_of_frames == 4:
            state = np.array([state, state, state, state])
        elif task.args.num_of_frames == 3:
            state = np.array([state, state, state])
        elif task.args.num_of_frames == 2:
            state = np.array([state, state])

        normalized_score = 0
        true_score = 0
        moves = 0
        wrong_moves = 0

        for t in range(task.max_steps):
            action = task.agent.get_action(state, epsilon=True)
            next_state, reward, done, info = task.env.step(action)

            true_score = true_score + reward

            if normalize_score and task.type != "basic":
                if reward > 0.0:
                    reward = 1.0
                else:
                    reward = 0.0
                if task.type == "text":
                    if sum(last_state) == sum(next_state):
                        reward = -0.1
                        wrong_moves = wrong_moves + 1

                if done:
                    reward = -1.0

            normalized_score = normalized_score + reward

            last_state = next_state
            if task.type == "text": 
                next_state = next_state / 16384
            elif task.type == "ram":
                next_state = next_state / 255
            elif task.type == "image":
                next_state = engineer_img(next_state)

            if task.args.num_of_frames == 4:
                next_state = np.array([state[1], state[2], state[3], next_state])
            elif task.args.num_of_frames == 3:
                next_state = np.array([state[1], state[2], next_state])
            elif task.args.num_of_frames == 2:
                next_state = np.array([state[1], next_state])

            steps = steps + 1
            moves = moves + 1
            
            task.agent.remember(state, action, reward, next_state, done, rand_agent=False)
            task.agent.train()

            task.agent.decrease_epsilon()
            state = next_state

            if steps % task.args.update_f == 0 and task.type != "basic":
                task.agent.update_target_net()

            if done:
                if eps % task.args.save_f == 0:
                    task.agent.save_model_weights("{}-{}.h5" .format(task.name, eps))
                
                if task.type == "basic":
                    task.agent.update_target_net()

                episodes_numbers.append(eps)
                true_scores.append(true_score)
                normalized_scores.append(normalized_score)

                if task.name == "2048-v0":
                    highest.append(task.env.unwrapped.highest())

                    print("Episode: {}/{}, Steps: {}, Memory: {}, Epsilon: {:.2}, Moves in episode: {}, Highest: {}, Normalized score: {:.4}, True score: {}" 
                          .format(eps, task.args.episodes-1, steps, len(task.agent.memory) if task.agent.memory_type == "basic" else task.agent.memory.length,
                                  task.agent.current_epsilon, moves, task.env.unwrapped.highest(), float(normalized_score), true_score), end="")
                else:
                    print("Episode: {}/{}, Steps: {}, Memory: {}, Epsilon: {:.2}, Moves in episode: {}, Normalized score: {}, True score: {}" 
                          .format(eps, task.args.episodes-1, steps, len(task.agent.memory) if task.agent.memory_type == "basic" else task.agent.memory.length,
                                  task.agent.current_epsilon, moves, normalized_score, true_score), end="")
                if task.name == "2048-v0":
                    print(", Wrong moves: {}" .format(wrong_moves))
                else:
                    print("\n", end="")

                if task.type == "basic":
                    task.test(true_scores, episodes_numbers)

                break

    return task, true_scores, episodes_numbers, highest, normalized_score

def main():
    """
    main
    """
    args = get_args()

    os.environ["TF_CPP_MIN_LOG_LEVEL"]="3"
    if args.process_unit == "CPU":
        config = tf.ConfigProto(
            device_count={'CPU' : 1, 'GPU' : 0},
            intra_op_parallelism_threads=1, 
            inter_op_parallelism_threads=1)
        set_session(tf.Session(config=config))
    else:
        config = tf.ConfigProto(
            device_count={'CPU' : 1, 'GPU' : 1})
        config.gpu_options.per_process_gpu_memory_fraction = 1
        set_session(tf.Session(config=config))

    task = Task(args)

    if task.args.init and task.args.model is None and task.args.mode == "train":
        if task.agent.memory_type == "basic":
            if task.type == "basic" or task.type == "text" or task.type == "ram":
                pl.rand_agent_replay_fs(task)
            elif task.type == "image":
                pl.rand_agent_replay_ps(task)
        else:
            if task.type == "basic" or task.type == "text" or task.type == "ram":
                pl.prior_rand_agent_replay_fs(task)
            elif task.type == "image":
                pl.prior_rand_agent_replay_ps(task)

    if task.args.model is not None:
        task.agent.load_model_weights("./{}" .format(task.args.model))
        task.agent.update_target_net()
        
        task.agent.current_epsilon = 0.1

        if task.args.init and task.args.mode == "train":
            if task.agent.memory_type == "basic":
                if task.type == "basic" or task.type == "text" or task.type == "ram":
                    pl.agent_replay_fs(task)
                elif task.type == "image":
                    pl.agent_replay_ps(task)
            else:
                if task.type == "basic" or task.type == "text" or task.type == "ram":
                    pl.prior_agent_replay_fs(task)
                elif task.type == "image":
                    pl.prior_agent_replay_ps(task)

    if task.args.vids:
        task.env = wrappers.Monitor(task.env, "./", video_callable=lambda episode_id: episode_id%1==0, force=True)

    if task.args.mode == "train":
        task, true_scores, episodes_numbers, highest, normalized_scores = train(task, True)
        task.agent.save_model_weights("{}-last.h5" .format(task.name))
        task.agent.save_target_weights("{}-last.h5" .format(task.name))
        try:
            combined_graph(true_scores, episodes_numbers, "{}_results" .format(task.name), [episodes_numbers[-1]+10,max(true_scores)+10],
                           {task.average_rand_score:task.average_rand_score}, True)
        except ValueError:
            err_print("[Not enough values to make graph.]")

    elif task.args.mode == "test":
        pl.agent_score_estimate(task, task.args.episodes, render=False, show_bar=True)

    elif task.args.mode == "render":
        pl.agent_score_estimate(task, task.args.episodes, render=True, show_bar=True)

    K.clear_session()

if __name__ == "__main__":
    main()
