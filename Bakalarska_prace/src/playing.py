"""
file contains methods for score estimation,
for fulling diffrent types of memory with diffrent sources
and processing of images
"""
import time
import sys
import scipy
import scipy.misc
import numpy as np
from tqdm import tqdm
from tqdm import trange

def engineer_img(img):
    """
    method proceses image - make it gray and normalize
    """
    img = scipy.misc.imresize(img, (84, 84), interp='bilinear') 

    red, green, blue = img[:,:,0], img[:,:,1], img[:,:,2]
    img = 0.299 * red + 0.587 * green + 0.114 * blue #RGB -> Luma (Digital ITU BT.601)

    img = img.astype(np.uint8) / 255.0 - 0.5 #normalization

    return img

class Playing():
    """
    class contains methods for score estimation and
    for fulling diffrent types of memory with diffrent sources
    """
    def rand_score_estimate(task, games):
        """
        score estimation for random agent
        """
        total_reward = 0

        bar = trange(games, leave=True)
        for game in bar:
            task.env.reset()
            done = False

            while not done:
                action = np.random.randint(0, task.env_action_size, size=1)[0]
                next_state, reward, done, info = task.env.step(action)
                total_reward = total_reward + reward

            if game != 0:
                bar.set_description("Average score: " + str(round(total_reward / (game+1), 2)))

        print("[Random agent average score: {}]" .format(round(total_reward / games, 2)))

        return total_reward / games

    def agent_score_estimate(task, games, render=False, show_bar=False):
        """
        score estimation for trained agent
        """
        total_reward = 0
        bar = trange(games, leave=True, disable=(not show_bar))
        for game in bar:
            state = task.env.reset()
            last_state = state
            wrong_move = False
            done = False

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

            while not done:
                if render and task.type == "text":
                    time.sleep(0.5)
                    task.env.render()
                elif render:
                    time.sleep(0.03)
                    task.env.render()
                    
                if task.name == "2048-v0" and wrong_move:
                    action = np.argmax(actions)
                    wrong_move = False
                else:
                    if task.name == "Breakout-v0" or task.name == "Breakout-ram-v0" or task.name == "BeamRider-v0":
                        action = task.agent.get_action(state, epsilon=True)
                    else:
                        actions = task.agent.model_net.predict(np.array([state]))
                        action = np.argmax(actions)

                next_state, reward, done, info = task.env.step(action)

                if task.name == "2048-v0":
                    if sum(last_state) == sum(next_state):
                        wrong_move = True
                        actions[0][action] = -10000

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

                state = next_state
                total_reward = total_reward + reward
            if game != 0:
                bar.set_description("Average score: " + str(round(total_reward / (game+1), 2)))

        if show_bar:
            print("[Agent's average score: {}]" .format(round(total_reward / games, 2)))

        return total_reward / games

    def prior_rand_agent_replay_fs(task, normalize_score=True):
        """
        random agent fulfills pririty experience replay memory without buffer
        """
        task.agent.clear_memory()
        new_observations = 0

        while True:
            state = task.env.reset()

            if task.type == "text": 
                state = state / 16384
            elif task.type == "ram":
                state = state / 255

            while True:
                action = np.random.randint(0, task.env_action_size, size=1)[0]
                next_state, reward, done, info = task.env.step(action)

                if normalize_score and task.type != "basic":
                    if reward > 0.0:
                        reward = 1.0
                    else:
                        reward = 0.0

                    if done:
                        reward = -1.0

                if task.type == "text": 
                    next_state = next_state / 16384
                elif task.type == "ram":
                    next_state = next_state / 255

                task.agent.remember(state, action, reward, next_state, done, rand_agent=True)
                new_observations = new_observations + 1
                state = next_state

                if task.agent.memory.length == task.agent.memory_size:
                    print("[Random agent added {} new memories. Current memory_size: {}]" 
                          .format(new_observations, len(task.agent.memory) if task.agent.memory_type == "basic" else task.agent.memory.length))
                    return new_observations

                if done:
                    break

    def prior_rand_agent_replay_ps(task, normalize_score=True):
        """
        random agent fulfills pririty experience replay memory with buffer
        """
        new_observations = 0
        task.agent.clear_memory()

        while True:
            state = task.env.reset()
            state = engineer_img(state)

            if task.args.num_of_frames == 4:
                state = np.array([state, state, state, state])
            elif task.args.num_of_frames == 3:
                state = np.array([state, state, state])
            elif task.args.num_of_frames == 2:
                state = np.array([state, state])

            while True:
                action = np.random.randint(0, task.env_action_size, size=1)[0]
                next_state, reward, done, info = task.env.step(action)

                if task.args.num_of_frames == 4:
                    next_state = np.array([state[1], state[2], state[3], engineer_img(next_state)])
                elif task.args.num_of_frames == 3:
                    next_state = np.array([state[1], state[2], engineer_img(next_state)])
                elif task.args.num_of_frames == 2:
                    next_state = np.array([state[1], engineer_img(next_state)])

                if normalize_score and task.type != "basic":
                    if reward > 0.0:
                        reward = 1.0
                    else:
                        reward = 0.0

                    if done:
                        reward = -1.0

                task.agent.remember(state, action, reward, next_state, done, rand_agent=True)
                new_observations = new_observations + 1

                state = next_state

                if task.agent.memory.length == task.agent.memory_size:
                    print("[Random agent added {} new memories. Current memory_size: {}]" 
                          .format(new_observations, len(task.agent.memory) if task.agent.memory_type == "basic" else task.agent.memory.length))
                    return new_observations

                if done:
                    break

    def prior_agent_replay_fs(task, normalize_score=True):
        """
        trained agent fulfills pririty experience replay memory without buffer
        """
        new_observations = 0
        average_priority = task.agent.memory.priority_tree[0] / task.agent.memory.length

        while True:
            state = task.env.reset()
            last_state = state
            wrong_move = False

            if task.type == "text": 
                state = state / 16384
            elif task.type == "ram":
                state = state / 255

            while True:
                if task.name == "2048-v0" and wrong_move:
                    action = np.random.randint(0, task.env_action_size, size=1)[0]
                    wrong_move = False
                else:
                    action = task.agent.get_action(state, epsilon=True)
                next_state, reward, done, info = task.env.step(action)

                if normalize_score and task.type != "basic":
                    if reward > 0.0:
                        reward = 1.0
                    else:
                        reward = 0.0

                    if done:
                        reward = -1.0

                if sum(last_state) == sum(next_state):
                    wrong_move = True

                last_state = next_state
                if task.type == "text": 
                    next_state = next_state / 16384
                elif task.type == "ram":
                    next_state = next_state / 255

                task.agent.remember(state, action, reward, next_state, done, rand_agent=False)
                new_observations = new_observations + 1
                state = next_state

                if task.agent.memory.length == task.agent.memory_size:
                    print("[Agent added {} new memories. Current memory_size: {}]" 
                          .format(new_observations, len(task.agent.memory) if task.agent.memory_type == "basic" else task.agent.memory.length))
                    return new_observations

                if done:
                    break

    def prior_agent_replay_ps(task, normalize_score=True):
        """
        trained agent fulfills pririty experience replay memory with buffer
        """
        new_observations = 0
        task.agent.clear_memory()

        while True:
            state = task.env.reset()
            state = engineer_img(state)

            if task.args.num_of_frames == 4:
                state = np.array([state, state, state, state])
            elif task.args.num_of_frames == 3:
                state = np.array([state, state, state])
            elif task.args.num_of_frames == 2:
                state = np.array([state, state])

            while True:
                action = task.agent.get_action(state, epsilon=True)
                next_state, reward, done, info = task.env.step(action)

                if task.args.num_of_frames == 4:
                    next_state = np.array([state[1], state[2], state[3], engineer_img(next_state)])
                elif task.args.num_of_frames == 3:
                    next_state = np.array([state[1], state[2], engineer_img(next_state)])
                elif task.args.num_of_frames == 2:
                    next_state = np.array([state[1], engineer_img(next_state)])

                if normalize_score and task.type != "basic":
                    if reward > 0.0:
                        reward = 1.0
                    else:
                        reward = 0.0

                    if done:
                        reward = -1.0

                task.agent.remember(state, action, reward, next_state, done, rand_agent=False)
                new_observations = new_observations + 1

                state = next_state

                if task.agent.memory.length == task.agent.memory_size:
                    print("[Agent added {} new memories. Current memory_size: {}]" 
                          .format(new_observations, len(task.agent.memory) if task.agent.memory_type == "basic" else task.agent.memory.length))
                    return new_observations

                if done:
                    break

    def rand_agent_replay_fs(task, normalize_score=True):
        """
        random agent fulfills experience replay memory without buffer
        """
        new_observations = 0
        task.agent.clear_memory()

        while True:
            state = task.env.reset()

            if task.type == "text": 
                state = state / 16384
            elif task.type == "ram":
                state = state / 255

            while True:
                action = np.random.randint(0, task.env_action_size, size=1)[0]
                next_state, reward, done, info = task.env.step(action)

                if normalize_score and task.type != "basic":
                    if reward > 0.0:
                        reward = 1.0
                    else:
                        reward = 0.0

                    if done:
                        reward = -1.0

                if task.type == "text": 
                    next_state = next_state / 16384
                elif task.type == "ram":
                    next_state = next_state / 255

                task.agent.remember(state, action, reward, next_state, done, rand_agent=True)
                new_observations = new_observations + 1
                state = next_state
                
                if len(task.agent.memory) == task.agent.memory_size:
                    print("[Random agent added {} new memories. Current memory_size: {}]" 
                          .format(new_observations, len(task.agent.memory) if task.agent.memory_type == "basic" else task.agent.memory.length))
                    return new_observations

                if done:
                    break


    def rand_agent_replay_ps(task, normalize_score=True):
        """
        random agent fulfills experience replay memory with buffer
        """
        new_observations = 0
        task.agent.clear_memory()

        while True:
            state = task.env.reset()
            state = engineer_img(state)

            if task.args.num_of_frames == 4:
                state = np.array([state, state, state, state])
            elif task.args.num_of_frames == 3:
                state = np.array([state, state, state])
            elif task.args.num_of_frames == 2:
                state = np.array([state, state])

            while True:
                action = np.random.randint(0, task.env_action_size, size=1)[0]
                next_state, reward, done, info = task.env.step(action)

                if task.args.num_of_frames == 4:
                    next_state = np.array([state[1], state[2], state[3], engineer_img(next_state)])
                elif task.args.num_of_frames == 3:
                    next_state = np.array([state[1], state[2], engineer_img(next_state)])
                elif task.args.num_of_frames == 2:
                    next_state = np.array([state[1], engineer_img(next_state)])

                if normalize_score and task.type != "basic":
                    if reward > 0.0:
                        reward = 1.0
                    else:
                        reward = 0.0

                    if done:
                        reward = -1.0

                task.agent.remember(state, action, reward, next_state, done, rand_agent=True)
                new_observations = new_observations + 1

                state = next_state

                if len(task.agent.memory) == task.agent.memory_size:
                    print("[Random agent added {} new memories. Current memory_size: {}]" 
                          .format(new_observations, len(task.agent.memory) if task.agent.memory_type == "basic" else task.agent.memory.length))
                    return new_observations

                if done:
                    break

    def agent_replay_fs(task, normalize_score=True):
        """
        trained agent fulfills experience replay memory without buffer
        """
        new_observations = 0
        task.agent.clear_memory()

        while True:
            state = task.env.reset()
            last_state = state
            wrong_move = False

            if task.type == "text": 
                state = state / 16384
            elif task.type == "ram":
                state = state / 255

            while True:
                if task.name == "2048-v0" and wrong_move:
                    action = np.random.randint(0, task.env_action_size, size=1)[0]
                    wrong_move = False
                else:
                    action = task.agent.get_action(state, epsilon=True)
                next_state, reward, done, info = task.env.step(action)

                if normalize_score and task.type != "basic":
                    if reward > 0.0:
                        reward = 1.0
                    else:
                        reward = 0.0

                    if done:
                        reward = -1.0
                        
                if sum(last_state) == sum(next_state):
                    wrong_move = True

                last_state = next_state
                if task.type == "text": 
                    next_state = next_state / 16384
                elif task.type == "ram":
                    next_state = next_state / 255

                task.agent.remember(state, action, reward, next_state, done, rand_agent=False)
                new_observations = new_observations + 1
                state = next_state
                
                if len(task.agent.memory) == task.agent.memory_size:
                    print("[Agent added {} new memories. Current memory_size: {}]" 
                          .format(new_observations, len(task.agent.memory) if task.agent.memory_type == "basic" else task.agent.memory.length))
                    return new_observations

                if done:
                    break

    def agent_replay_ps(task, normalize_score=True):
        """
        trained agent fulfills experience replay memory with buffer
        """
        new_observations = 0
        task.agent.clear_memory()

        while True:
            state = task.env.reset()
            state = engineer_img(state)

            if task.args.num_of_frames == 4:
                state = np.array([state, state, state, state])
            elif task.args.num_of_frames == 3:
                state = np.array([state, state, state])
            elif task.args.num_of_frames == 2:
                state = np.array([state, state])

            while True:
                action = task.agent.get_action(state, epsilon=True)
                next_state, reward, done, info = task.env.step(action)

                if task.args.num_of_frames == 4:
                    next_state = np.array([state[1], state[2], state[3], engineer_img(next_state)])
                elif task.args.num_of_frames == 3:
                    next_state = np.array([state[1], state[2], engineer_img(next_state)])
                elif task.args.num_of_frames == 2:
                    next_state = np.array([state[1], engineer_img(next_state)])

                if normalize_score and task.type != "basic":
                    if reward > 0.0:
                        reward = 1.0
                    else:
                        reward = 0.0

                    if done:
                        reward = -1.0

                task.agent.remember(state, action, reward, next_state, done, rand_agent=False)
                new_observations = new_observations + 1

                state = next_state

                if len(task.agent.memory) == task.agent.memory_size:
                    print("[Agent added {} new memories. Current memory_size: {}]" 
                          .format(new_observations, len(task.agent.memory) if task.agent.memory_type == "basic" else task.agent.memory.length))
                    return new_observations

                if done:
                    break
