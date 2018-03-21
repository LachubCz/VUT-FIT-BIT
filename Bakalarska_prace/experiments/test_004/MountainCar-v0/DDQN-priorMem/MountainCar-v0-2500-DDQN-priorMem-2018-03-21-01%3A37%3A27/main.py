#!/usr/bin/env python
"""
docstring
"""
import os
import numpy as np
import tensorflow as tf
from keras.backend.tensorflow_backend import set_session

from task import Task
from visualization import point_graph, gaussian_graph, combined_graph
from playing import Playing as pl
from profiling import * #profiling - @do_profile(follow=[method, ])

#@do_profile(follow=[pl.rand_agent_replay])
def main():
    """
    docstring
    """
    os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
    config = tf.ConfigProto(intra_op_parallelism_threads=1, inter_op_parallelism_threads=1)
    set_session(tf.Session(config=config))

    episodes = 2500
    scores = []
    episodes_numbers = []
    task = Task("MountainCar-v0", "DDQN", model_type="basic", memory_type="priority")
    #task.env = wrappers.Monitor(env, '/home/lachubcz/tmp/cartpole-experiment-1', force=True)

    if task.agent.memory_type == "basic":
        new_memories = pl.rand_agent_replay(task, 10000, task.max_steps)
    else:
        new_memories = pl.prior_rand_agent_replay(task, 10000, task.max_steps)

    print("Random agent added {} new memories. Current memory_size: {}" 
          .format(new_memories, len(task.agent.memory) if task.agent.memory_type == "basic" else task.agent.memory.length))

    #task.agent.load_model_weights("./MountainCar-v0-solved.h5")
    #task.agent.load_target_weights("./target-CartPole-v0-0.h5")
    #task.agent.update_target_net()

    for eps in range(episodes):
        state = task.env.reset()

        for t in range(task.max_steps):
            state = np.reshape(state, (1, task.env_state_size))
            action = task.agent.get_action(state, epsilon=True)
            next_state, reward, done, info = task.env.step(action)

            task.agent.remember(state, action, reward, next_state, done, rand_agent=False)
            task.agent.train()

            state = next_state

            task.agent.decrease_epsilon()
            if done:
                score = pl.score_estimate(task, 1)
                scores.append(score)
                episodes_numbers.append(eps)
                if task.agent.memory_type == "basic":
                    print("Episode: {}/{}, epsilon: {:.2}, memory_size: {}, score: {}"
                          .format(eps, episodes, task.agent.current_epsilon, len(task.agent.memory), score))
                else:
                    print("Episode: {}/{}, epsilon: {:.2}, memory_size: {}, priority_tree: {:.2}, score: {}"
                          .format(eps, episodes, task.agent.current_epsilon, task.agent.memory.length, task.agent.memory.priority_tree[0], score))

                if score >= task.solved_score:
                    task.test(eps, scores, episodes_numbers)

                #if eps % 25 == 0:
                    #if eps != 0:
                    #    before = task.agent.memory.priority_tree[0]
                    #    added = agent_replay(task, int((task.agent.memory_size / 10) / (task.max_steps * 2)),
                    #                         task.max_steps, (task.agent.memory_size / 10))
                    #    after = task.agent.memory.priority_tree[0]
                    #    print("Added {} new memories. Value of priority_tree was {} and now is {}." .format(added, before, after))

                    #task.agent.save_model_weights("{}-{}.h5" .format(task.name, eps))

                task.agent.update_target_net()
                break

    task.agent.save_model_weights("{}-last.h5" .format(task.name))
    point_graph(scores, episodes_numbers, "{}-point_graph.png" .format(task.name))
    gaussian_graph(scores, episodes_numbers, "{}-gaussian_graph.png" .format(task.name))
    combined_graph(scores, episodes_numbers, "{}-combined_graph.png" .format(task.name))

main()
