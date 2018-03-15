import numpy as np
from memory import *

def rand_score_estimate(task, games):
    
    total_reward = 0
    
    for game in range(games):

        state = task.env.reset()
        
        done = False
        
        while not done:

            action = np.random.randint(0, task.env_action_size, size=1)[0]

            nextState, reward, done, info = task.env.step(action)
            
            total_reward = total_reward + reward

    return (total_reward / games)

def score_estimate(task, games):
    
    total_reward = 0
    
    for game in range(games):

        state = task.env.reset()
        
        done = False
        
        while not done:

            state = np.reshape(state, (1, task.env_state_size))
            
            action = task.agent.get_action(state) 

            nextState, reward, done, info = task.env.step(action)

            state = nextState
            
            total_reward = total_reward + reward

    return (total_reward / games)

def rand_agent_replay(task, episodes, observetime):
    
    task.agent.clear_memory()

    for eps in range(episodes):

        state = task.env.reset()

        for t in range(observetime):

            action = np.random.randint(0, task.env_action_size, size=1)[0]

            nextState, reward, done, info = task.env.step(action)

            task.agent.remember(state, action, reward, nextState, done, rand_agent=True)

            state = nextState

            if task.agent.memory.length == task.agent.memory_size:
                return

            if done:
                break

def agent_replay(task, episodes, observetime):
    
    task.agent.clear_memory()

    new_observation = 0

    average_priority = task.agent.memory.priority_tree[0] / task.agent.memory.length

    for eps in range(episodes):

        state = task.env.reset()

        for t in range(observetime):

            state = np.reshape(state, (1, task.env_state_size))
            action = task.agent.getActionWE(state)

            nextState, reward, done, info = task.env.step(action)

            obs_error = task.agent.get_error(state, action, reward, nextState, done)
            if task.agent.memory.get_priority(obs_error) > average_priority:
                task.agent.memory.add_observation((state, action, reward, nextState, done), obs_error)
                new_observation = new_observation + 1

            state = nextState
            
            #if task.agent.memory.length == task.agent.memory_size:
            #    return new_observation

            if done:
                break

    return new_observation

def weights_test(task, games, path):
    task.agent.loadNN("{}" .format(path))

    avg_score = score_estimate(task, games)

    return avg_score