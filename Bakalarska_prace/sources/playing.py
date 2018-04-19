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
        odhad nahodneho skore pro vectorove hry
        """
        total_reward = 0

        for game in range(games):
            done = False

            while not done:
                action = np.random.randint(0, task.env_action_size, size=1)[0]
                next_state, reward, done, info = task.env.step(action)

                total_reward = total_reward + reward

        return total_reward / games

    def score_estimate_vect(task, games):
        """
        odhad agentova skore pro vektorove hry
        """
        total_reward = 0

        for game in range(games):
            state = task.env.reset()
            done = False

            while not done:

                action = task.agent.get_action(state, epsilon=False)
                next_state, reward, done, info = task.env.step(action)

                state = next_state
                total_reward = total_reward + reward

        return total_reward / games

    def score_estimate_img(task, games):
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

    def prior_rand_agent_replay_vect(task, episodes, observetime):
        """
        nahodny agent s prioritni pameti a vektorovym prostredim
        """
        task.agent.clear_memory()
        new_observation = 0

        for eps in range(episodes):
            state = task.env.reset()

            for t in range(observetime):
                action = np.random.randint(0, task.env_action_size, size=1)[0]
                next_state, reward, done, info = task.env.step(action)

                task.agent.remember(state, action, reward, next_state, done, rand_agent=True)
                new_observation = new_observation + 1
                state = next_state

                if task.agent.memory.length == task.agent.memory_size:
                    return new_observation

                if done:
                    break

    def prior_rand_agent_replay_img(task, episodes, observetime):
        """
        nahodny agent s prioritni pameti a obrazovym prostredim
        """
        new_observation = 0
        task.agent.clear_memory()

        for eps in range(episodes):
            state = task.env.reset()
            state = engineer_img(state)
            state = np.array([state, state])

            for t in range(task.max_steps):
                action = np.random.randint(0, task.env_action_size, size=1)[0]
                next_state, reward, done, info = task.env.step(action)

                next_state = np.array([state[1], engineer_img(next_state)])

                task.agent.remember(state, action, reward, next_state, done, rand_agent=True)
                new_observation = new_observation + 1

                state = next_state

                if task.agent.memory.length == task.agent.memory_size:
                    return new_observation

                if done:
                    break

    def prior_agent_replay_vect(task, episodes, observetime, max_new_samples):
        """
        nenahodny agent s normalni pameti a vektorovym prostredim
        """
        new_observation = 0
        #task.agent.clear_memory()
        average_priority = task.agent.memory.priority_tree[0] / task.agent.memory.length

        for eps in range(episodes):
            state = task.env.reset()

            for t in range(observetime):
                state = np.reshape(state, (1, task.env_state_size))
                action = task.agent.get_action(state, epsilon=True)
                next_state, reward, done, info = task.env.step(action)

                obs_error = task.agent.get_error(state, action, reward, next_state, done)

                if task.agent.memory.get_priority(obs_error) > average_priority:
                    task.agent.memory.add_observation((state, action, reward, next_state, done), obs_error)
                    new_observation = new_observation + 1

                state = next_state

                if new_observation == max_new_samples:
                    return new_observation

                if done:
                    break

        return new_observation

    def prior_agent_replay_img(task, episodes, observetime):
        """
        nenahodny agent s prioritni pameti a obrazovym prostredim NEDOKONCENO
        """
        new_observation = 0
        task.agent.clear_memory()

        for eps in range(episodes):
            state = task.env.reset()
            state = engineer_img(state)
            state = np.array([state, state])

            for t in range(task.max_steps):
                action = task.agent.get_action(state, epsilon=True)
                next_state, reward, done, info = task.env.step(action)
                next_state = np.array([state[1], engineer_img(next_state)])
        
                task.agent.remember(state, action, reward, next_state, done, rand_agent=False)
                new_observation = new_observation + 1

                state = next_state

                if task.agent.memory.length == task.agent.memory_size:
                    return new_observation

                if done:
                    break

    def rand_agent_replay_vect(task, episodes, observetime):
        """
        nahodny agent s normalni pameti a vektorovym prostredim
        """
        new_observation = 0
        task.agent.clear_memory()

        for eps in range(episodes):
            state = task.env.reset()

            for t in range(observetime):
                action = np.random.randint(0, task.env_action_size, size=1)[0]
                next_state, reward, done, info = task.env.step(action)

                task.agent.remember(state, action, reward, next_state, done, rand_agent=True)
                new_observation = new_observation + 1
                state = next_state
                
                if len(task.agent.memory) == task.agent.memory_size:
                    return new_observation

                if done:
                    break


    def rand_agent_replay_img(task, episodes, observetime):
        """
        nahodny agent s normalni pameti a obrazovym prostredim
        """
        new_observation = 0
        task.agent.clear_memory()

        for eps in range(episodes):
            state = task.env.reset()
            state = engineer_img(state)
            state = np.array([state, state])

            for t in range(task.max_steps):
                action = np.random.randint(0, task.env_action_size, size=1)[0]
                next_state, reward, done, info = task.env.step(action)
                next_state = np.array([state[1], engineer_img(next_state)])
        
                task.agent.remember(state, action, reward, next_state, done, rand_agent=True)
                new_observation = new_observation + 1

                state = next_state

                if len(task.agent.memory) == task.agent.memory_size:
                    return new_observation

                if done:
                    break

    def agent_replay_vect(task, episodes, observetime):
        """
        nenahodny agent s normalni pameti a vektorovym prostredim
        """
        new_observation = 0
        task.agent.clear_memory()

        for eps in range(episodes):
            state = task.env.reset()

            for t in range(observetime):
                state = np.reshape(state, (1, task.env_state_size))
                action = task.agent.get_action(state, epsilon=True)
                next_state, reward, done, info = task.env.step(action)

                task.agent.remember(state, action, reward, next_state, done, rand_agent=False)
                new_observation = new_observation + 1
                state = next_state
                
                if len(task.agent.memory) == task.agent.memory_size:
                    return new_observation

                if done:
                    break

    def agent_replay_img(task, episodes, observetime):
        """
        nenahodny agent s normalni pameti a obrazovym prostredim
        """
        new_observation = 0
        task.agent.clear_memory()

        for eps in range(episodes):
            state = task.env.reset()
            state = engineer_img(state)
            state = np.array([state, state])

            for t in range(task.max_steps):
                action = task.agent.get_action(state, epsilon=True)
                next_state, reward, done, info = task.env.step(action)
                next_state = np.array([state[1], engineer_img(next_state)])
        
                task.agent.remember(state, action, reward, next_state, done, rand_agent=False)
                new_observation = new_observation + 1

                state = next_state

                if len(task.agent.memory) == task.agent.memory_size:
                    return new_observation

                if done:
                    break

    def weights_test(task, games, path):
        """
        docstring
        """
        task.agent.loadNN("{}" .format(path))
        avg_score = score_estimate(task, games)

        return avg_score
