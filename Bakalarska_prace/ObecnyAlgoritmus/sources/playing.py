import numpy as np

def score_estimate(env, agent, games, stateSize):
    
    total_reward = 0
    
    for game in range(games):

        state = env.reset()
        
        done = False
        
        while not done:

            state = np.reshape(state, (1, stateSize))
            
            action = agent.get_action(state) 

            nextState, reward, done, info = env.step(action)

            state = nextState
            
            total_reward = total_reward + reward

    return (total_reward / games)

def rand_agent_replay(env, agent, episodes, observetime, actionCount):
    
    agent.clear_memory()

    for eps in range(episodes):

        state = env.reset()

        for t in range(observetime):

            action = np.random.randint(0, actionCount, size=1)[0]

            nextState, reward, done, info = env.step(action)

            agent.remember(state, action, reward, nextState, done)

            state = nextState
            
            if len(agent.memory) == agent.memory_size:
                return

            if done:
                break

def agent_replay(env, agent, episodes, observetime, stateSize):
    
    agent.clear_memory()

    for eps in range(episodes):

        state = env.reset()

        for t in range(observetime):

            state = np.reshape(state, (1, stateSize))
            action = agent.get_action(state)

            nextState, reward, done, info = env.step(action)

            agent.remember(state, action, reward, nextState, done)

            state = nextState
            
            if len(agent.memory) == agent.memory_size:
                return

            if done:
                break

def weights_test(env, agent, games, state_size, path):
    agent.loadNN("{}" .format(path))

    avg_score = score_estimate(env, agent, games, state_size)

    return avg_score