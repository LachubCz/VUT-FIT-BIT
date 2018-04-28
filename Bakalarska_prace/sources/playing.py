"""
docstring
"""
import scipy
import numpy as np

def engineer_img(img):
    """
    docstring
    """
    img = scipy.misc.imresize(img, (84, 84), interp='bilinear') 

    red, green, blue = img[:,:,0], img[:,:,1], img[:,:,2]
    img = 0.299 * red + 0.587 * green + 0.114 * blue #RGB -> Luma (Digital ITU BT.601)

    img = img.astype(np.uint8) / 255.0 - 0.5 #normalizace
    return img

class Playing():
    """
    docstring
    """
    def rand_score_estimate(task, games):
        """
        odhad nahodneho skore pro fsorove hry
        """
        total_reward = 0

        for game in range(games):
            task.env.reset()
            done = False

            while not done:
                action = np.random.randint(0, task.env_action_size, size=1)[0]
                next_state, reward, done, info = task.env.step(action)
                total_reward = total_reward + reward

        return total_reward / games

    def score_estimate_fs(task, games):
        """
        odhad agentova skore pro vektorove hry
        """
        total_reward = 0

        for game in range(games):
            state = task.env.reset()
            done = False

            if task.type == "text": 
                state = state / 16384
            elif task.type == "ram":
                state = state / 255

            while not done:

                action = task.agent.get_action(state, epsilon=False)
                next_state, reward, done, info = task.env.step(action)

                if task.type == "text": 
                    next_state = next_state / 16384
                elif task.type == "ram":
                    next_state = next_state / 255

                state = next_state
                total_reward = total_reward + reward

        return total_reward / games

    def score_estimate_ps(task, games):
        """
        odhad agentova skore pro obrazove hry
        """
        total_reward = 0

        for game in range(games):
            state = task.env.reset()
            state = engineer_img(state)
            state = np.array([state, state])
            done = False

            while not done:
                action = task.agent.get_action(state, epsilon=False)
                next_state, reward, done, info = task.env.step(action)

                next_state = np.array([state[1], engineer_img(next_state)])

                state = next_state

                total_reward = total_reward + reward

        return total_reward / games

    def prior_rand_agent_replay_fs(task, normalize_score=True):
        """
        nahodny agent s prioritni pameti a vektorovym prostredim
        """
        task.agent.clear_memory()
        new_observation = 0

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
                        reward = 1
                    else:
                        reward = 0

                    if done:
                        reward = -1

                if task.type == "text": 
                    next_state = next_state / 16384
                elif task.type == "ram":
                    next_state = next_state / 255

                task.agent.remember(state, action, reward, next_state, done, rand_agent=True)
                new_observation = new_observation + 1
                state = next_state

                if task.agent.memory.length == task.agent.memory_size:
                    return new_observation

                if done:
                    break

    def prior_rand_agent_replay_ps(task, normalize_score=True):
        """
        nahodny agent s prioritni pameti a obrazovym prostredim
        """
        new_observation = 0
        task.agent.clear_memory()

        while True:
            state = task.env.reset()
            state = engineer_img(state)
            state = np.array([state, state])

            while True:
                action = np.random.randint(0, task.env_action_size, size=1)[0]
                next_state, reward, done, info = task.env.step(action)

                if normalize_score and task.type != "basic":
                    if reward > 0.0:
                        reward = 1
                    else:
                        reward = 0

                    if done:
                        reward = -1

                next_state = np.array([state[1], engineer_img(next_state)])

                task.agent.remember(state, action, reward, next_state, done, rand_agent=True)
                new_observation = new_observation + 1

                state = next_state

                if task.agent.memory.length == task.agent.memory_size:
                    return new_observation

                if done:
                    break

    def prior_agent_replay_fs(task, normalize_score=True):
        """
        nenahodny agent s normalni pameti a vektorovym prostredim
        """
        new_observation = 0
        average_priority = task.agent.memory.priority_tree[0] / task.agent.memory.length

        while True:
            state = task.env.reset()

            if task.type == "text": 
                state = state / 16384
            elif task.type == "ram":
                state = state / 255

            while True:
                action = task.agent.get_action(state, epsilon=True)
                next_state, reward, done, info = task.env.step(action)

                if normalize_score and task.type != "basic":
                    if reward > 0.0:
                        reward = 1
                    else:
                        reward = 0

                    if done:
                        reward = -1

                if task.type == "text": 
                    next_state = next_state / 16384
                elif task.type == "ram":
                    next_state = next_state / 255

                task.agent.remember(state, action, reward, next_state, done, rand_agent=False)
                new_observation = new_observation + 1
                state = next_state

                if task.agent.memory.length == task.agent.memory_size:
                    return new_observation

                if done:
                    break

    def prior_agent_replay_ps(task, normalize_score=True):
        """
        nenahodny agent s prioritni pameti a obrazovym prostredim NEDOKONCENO
        """
        new_observation = 0
        task.agent.clear_memory()

        while True:
            state = task.env.reset()
            state = engineer_img(state)
            state = np.array([state, state])

            while True:
                action = task.agent.get_action(state, epsilon=True)
                next_state, reward, done, info = task.env.step(action)
                next_state = np.array([state[1], engineer_img(next_state)])

                if normalize_score and task.type != "basic":
                    if reward > 0.0:
                        reward = 1
                    else:
                        reward = 0

                    if done:
                        reward = -1

                task.agent.remember(state, action, reward, next_state, done, rand_agent=False)
                new_observation = new_observation + 1

                state = next_state

                if task.agent.memory.length == task.agent.memory_size:
                    return new_observation

                if done:
                    break

    def rand_agent_replay_fs(task, normalize_score=True):
        """
        nahodny agent s normalni pameti a vektorovym prostredim
        """
        new_observation = 0
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
                        reward = 1
                    else:
                        reward = 0

                    if done:
                        reward = -1

                if task.type == "text": 
                    next_state = next_state / 16384
                elif task.type == "ram":
                    next_state = next_state / 255

                task.agent.remember(state, action, reward, next_state, done, rand_agent=True)
                new_observation = new_observation + 1
                state = next_state
                
                if len(task.agent.memory) == task.agent.memory_size:
                    return new_observation

                if done:
                    break


    def rand_agent_replay_ps(task, normalize_score=True):
        """
        nahodny agent s normalni pameti a obrazovym prostredim
        """
        new_observation = 0
        task.agent.clear_memory()

        while True:
            state = task.env.reset()
            state = engineer_img(state)
            state = np.array([state, state])

            while True:
                action = np.random.randint(0, task.env_action_size, size=1)[0]
                next_state, reward, done, info = task.env.step(action)
                next_state = np.array([state[1], engineer_img(next_state)])

                if normalize_score and task.type != "basic":
                    if reward > 0.0:
                        reward = 1
                    else:
                        reward = 0

                    if done:
                        reward = -1

                task.agent.remember(state, action, reward, next_state, done, rand_agent=True)
                new_observation = new_observation + 1

                state = next_state

                if len(task.agent.memory) == task.agent.memory_size:
                    return new_observation

                if done:
                    break

    def agent_replay_fs(task, normalize_score=True):
        """
        nenahodny agent s normalni pameti a vektorovym prostredim
        """
        new_observation = 0
        task.agent.clear_memory()

        while True:
            state = task.env.reset()

            if task.type == "text": 
                state = state / 16384
            elif task.type == "ram":
                state = state / 255

            while True:
                action = task.agent.get_action(state, epsilon=True)
                next_state, reward, done, info = task.env.step(action)

                if normalize_score and task.type != "basic":
                    if reward > 0.0:
                        reward = 1
                    else:
                        reward = 0

                    if done:
                        reward = -1

                if task.type == "text": 
                    next_state = next_state / 16384
                elif task.type == "ram":
                    next_state = next_state / 255

                task.agent.remember(state, action, reward, next_state, done, rand_agent=False)
                new_observation = new_observation + 1
                state = next_state
                
                if len(task.agent.memory) == task.agent.memory_size:
                    return new_observation

                if done:
                    break

    def agent_replay_ps(task, normalize_score=True):
        """
        nenahodny agent s normalni pameti a obrazovym prostredim
        """
        new_observation = 0
        task.agent.clear_memory()

        while True:
            state = task.env.reset()
            state = engineer_img(state)
            state = np.array([state, state])

            while True:
                action = task.agent.get_action(state, epsilon=True)
                next_state, reward, done, info = task.env.step(action)
                next_state = np.array([state[1], engineer_img(next_state)])

                if normalize_score and task.type != "basic":
                    if reward > 0.0:
                        reward = 1
                    else:
                        reward = 0

                    if done:
                        reward = -1

                task.agent.remember(state, action, reward, next_state, done, rand_agent=False)
                new_observation = new_observation + 1

                state = next_state

                if len(task.agent.memory) == task.agent.memory_size:
                    return new_observation

                if done:
                    break
