"""
docstring
"""
import numpy as np

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

def score_estimate(task, games):
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

def rand_agent_replay(task, episodes, observetime):
    """
    docstring
    """
    task.agent.clear_memory()

    for eps in range(episodes):
        state = task.env.reset()

        for t in range(observetime):
            action = np.random.randint(0, task.env_action_size, size=1)[0]
            next_state, reward, done, info = task.env.step(action)

            task.agent.remember(state, action, reward, next_state, done, rand_agent=True)
            state = next_state

            if task.agent.memory.length == task.agent.memory_size:
                return

            if done:
                break

def agent_replay(task, episodes, observetime, max_new_samples):
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

def weights_test(task, games, path):
    """
    docstring
    """
    task.agent.loadNN("{}" .format(path))
    avg_score = score_estimate(task, games)

    return avg_score
