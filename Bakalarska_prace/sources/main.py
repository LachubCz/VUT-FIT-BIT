#!/usr/bin/env python
"""
docstring
"""
import os
import os.path
import argparse
import scipy
import numpy as np
import tensorflow as tf
from keras.backend.tensorflow_backend import set_session

from task import Task
from visualization import point_graph, gaussian_graph, combined_graph
from playing import Playing as pl
from playing import processImage
from agent import Agent
from profiling import * #profiling - @do_profile(follow=[method, ])

def scoring(task, scores, episodes_numbers, eps, test_type="always", test_freq=None, save_freq=None, refresh_freq=None):
    """
    method for tuning different variables after game reached finite state
    """
    if test_freq is not None:
        if eps % test_freq == 0:
            score = pl.score_estimate_vect(task, 1)
            scores.append(score)
            episodes_numbers.append(eps)

            if test_type == "always":
                task.test(scores, episodes_numbers)
            else:
                if score >= task.solved_score:
                    task.test(scores, episodes_numbers)
            
            if task.agent.memory_type == "basic":
                print("Episode: {}/{}, epsilon: {:.2}, memory_size: {}, score: {}"
                      .format(eps+1, task.args.episodes, task.agent.current_epsilon, 
                              len(task.agent.memory), score))
            else:
                print("Episode: {}/{}, epsilon: {:.2}, memory_size: {}, priority_tree: {:.2}, score: {}"
                      .format(eps+1, task.args.episodes, task.agent.current_epsilon, 
                              task.agent.memory.length, task.agent.memory.priority_tree[0], score))
            
    if refresh_freq is not None:
        if task.agent.memory_type == "basic":
            if eps % refresh_freq == 0:
                if scores[-1] > scores[-2] and eps != 0:
                    agent_replay_vect(task, 10000, task.max_steps)
                    print("Memory refresh.")
        else:
            if eps % refresh_freq == 0:
                if episodes_numbers[-1] != 0:
                    before = task.agent.memory.priority_tree[0]
                    added = prior_agent_replay_vect(task, int((task.agent.memory_size / 10) / (task.max_steps * 2)),
                                         task.max_steps, (task.agent.memory_size / 10))
                    after = task.agent.memory.priority_tree[0]
                    print("Added {} new memories. Value of priority_tree was {} and now is {}." .format(added, before, after))

    if save_freq is not None:
        if episodes_numbers[-1] % save_freq == 0:
            task.agent.save_model_weights("{}-{}.h5" .format(task.name, episodes_numbers[-1]))
            print("Model was saved.")

    return scores, episodes_numbers
    
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
                        help="name of game environment")
    parser.add_argument("-eps", action="store", dest="episodes", type=int, required=True,
                        help="number of training episodes")
    parser.add_argument("-mdl", action="store", dest="model", help="name of file which contains already trained model to load")
    parser.add_argument("-pu", action="store", dest="process_unit", choices=["CPU", "GPU"], default="CPU", 
                        help="script will run on this processing unit")
    parser.add_argument("-vids", action="store_true", default=False,
                        help="script will record videos using gym.wrappers")

    args = parser.parse_args()
    
    if args.model is not None:
        if not os.path.isfile(args.model):
            err_print("Model file doesn't exist.")
            sys.exit(-1)

    return args

def vect_game(task, args):
    scores = []
    episodes_numbers = []

    for eps in range(args.episodes):
        state = task.env.reset()

        for t in range(task.max_steps):
            action = task.agent.get_action(state, epsilon=True)
            next_state, reward, done, info = task.env.step(action)

            task.agent.remember(state, action, reward, next_state, done, rand_agent=False)
            task.agent.train()

            state = next_state

            task.agent.decrease_epsilon()
            if done:
                scores, episodes = scoring(task, scores, episodes_numbers, eps, 
                    test_type="always", test_freq=1, save_freq=None, refresh_freq=None)

                task.agent.update_target_net()

                break

    return task, scores, episodes_numbers

def img_game(task, args):
    scores = []
    episodes_numbers = []

    for eps in range(args.episodes):
        state = task.env.reset()
        state = processImage(state)
        state = np.array([state, state])

        score = 0

        for t in range(task.max_steps):
            action = task.agent.get_action(state, epsilon=True)
            next_state, reward, done, info = task.env.step(action)
            next_state = np.array([state[1], processImage(next_state)])

            score = score + reward

            task.agent.remember(state, action, reward, next_state, done, rand_agent=False)
            task.agent.train()

            state = next_state

            task.agent.decrease_epsilon()
            if done:
                scores.append(score)
                episodes_numbers.append(eps)

                if task.agent.memory_type == "basic":
                    print("Episode: {}/{}, epsilon: {:.2}, memory_size: {}, score: {}"
                          .format(eps+1, task.args.episodes, task.agent.current_epsilon, 
                                  len(task.agent.memory), score))
                else:
                    print("Episode: {}/{}, epsilon: {:.2}, memory_size: {}, priority_tree: {:.2}, score: {}"
                          .format(eps+1, task.args.episodes, task.agent.current_epsilon, 
                                  task.agent.memory.length, task.agent.memory.priority_tree[0], score))

                scoring(task, scores, episodes_numbers, eps, test_type="always", test_freq=None, save_freq=10, refresh_freq=None)

                task.agent.update_target_net()
                break
    return task, scores, episodes_numbers

def main():
    """
    docstring
    """
    args = get_args()

    os.environ["TF_CPP_MIN_LOG_LEVEL"]="3"
    if args.process_unit == "CPU":
        config = tf.ConfigProto(intra_op_parallelism_threads=1, inter_op_parallelism_threads=1)
        set_session(tf.Session(config=config))
    else:
        config = tf.ConfigProto()
        config.gpu_options.per_process_gpu_memory_fraction = 1
        set_session(tf.Session(config=config))

    task = Task(args)

    if args.vids:
        task.env = wrappers.Monitor(task.env, "./", force=True)

    if args.init:
        if task.agent.memory_type == "basic":
            if task.type == "vect":
                new_memories = pl.rand_agent_replay_vect(task, 10000, task.max_steps)
            else:
                new_memories = pl.rand_agent_replay_img(task, 10000, task.max_steps)
        else:
            if task.type == "vect":
                new_memories = pl.prior_rand_agent_replay_vect(task, 10000, task.max_steps)
            else:
                new_memories = pl.prior_rand_agent_replay_img(task, 10000, task.max_steps)

        print("Random agent added {} new memories. Current memory_size: {}" 
              .format(new_memories, len(task.agent.memory) if task.agent.memory_type == "basic" else task.agent.memory.length))

    if args.model is not None:
        task.agent.load_model_weights("./{}" .format(args.mdl))
        task.agent.update_target_net()

    if task.type == "vect":
        task, scores, episodes_numbers = vect_game(task, args)
    else:
        task, scores, episodes_numbers = img_game(task, args)

    task.agent.save_model_weights("{}-last.h5" .format(task.name))
    point_graph(scores, episodes_numbers, "{}-point_graph.png" .format(task.name))
    gaussian_graph(scores, episodes_numbers, "{}-gaussian_graph.png" .format(task.name))
    combined_graph(scores, episodes_numbers, "{}-combined_graph.png" .format(task.name))

main()
