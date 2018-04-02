"""
docstring
"""
import scipy
import numpy as np

def processImage(img):
    rgb = scipy.misc.imresize(img, (84, 84), interp='bilinear')
    r, g, b = rgb[:,:,0], rgb[:,:,1], rgb[:,:,2]
    gray = 0.2989 * r + 0.5870 * g + 0.1140 * b     # extract luminance
    o = gray.astype('float32') / 128 - 1    # normalize
    return o

class Playing():
    """
    docstring
    """
    def rand_score_estimate(task, games):
        """
        docstring
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
        docstring
        """
        total_reward = 0

        for game in range(games):
            state = task.env.reset()
            done = False

            while not done:

                state = np.reshape(state, (1, task.env_state_size))
                action = task.agent.get_action(state, epsilon=False)
                next_state, reward, done, info = task.env.step(action)

                state = next_state
                total_reward = total_reward + reward

        return total_reward / games

    def prior_rand_agent_replay_vect(task, episodes, observetime):
        """
        docstring
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
        docstring
        """
        new_observation = 0
        task.agent.clear_memory()

        for eps in range(episodes):
            state = task.env.reset()
            state = processImage(state)
            state = np.array([state, state])

            for t in range(observetime):
                action = np.random.randint(0, task.env_action_size, size=1)[0]
                next_state, reward, done, info = task.env.step(action)

                next_state = np.array([state[1], processImage(next_state)])

                if done:
                    next_state = None

                task.agent.remember(state, action, reward, next_state, done, rand_agent=True)
                new_observation = new_observation + 1

                state = next_state

                if task.agent.memory.length == task.agent.memory_size:
                    return new_observation

                if done:
                    break

    def prior_agent_replay_vect(task, episodes, observetime, max_new_samples):
        """
        docstring
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

    def rand_agent_replay_vect(task, episodes, observetime):
        """
        docstring
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
        docstring
        """
        new_observation = 0
        task.agent.clear_memory()

        for eps in range(episodes):
            state = task.env.reset()
            state = processImage(state)
            state = np.array([state, state])

            for t in range(task.max_steps):
                action = np.random.randint(0, task.env_action_size, size=1)[0]
                next_state, reward, done, info = task.env.step(action)
                next_state = np.array([state[1], processImage(next_state)])
        
                task.agent.remember(state, action, reward, next_state, done, rand_agent=True)
                new_observation = new_observation + 1

                state = next_state

                if len(task.agent.memory) == task.agent.memory_size:
                    return new_observation

                if done:
                    break

    def agent_replay_vect(task, episodes, observetime):
        """
        docstring
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

    def weights_test(task, games, path):
        """
        docstring
        """
        task.agent.loadNN("{}" .format(path))
        avg_score = score_estimate(task, games)

        return avg_score
