import os
import copy
import matplotlib.pyplot as plt
from scipy.ndimage.filters import gaussian_filter

def point_graph(scores, episodes_numbers, name):
    fig = plt.figure()
    plt.plot(episodes_numbers, scores, 'ro')
    plt.ylabel("Skore")
    plt.xlabel("Epizody")
    plt.title("Graf agentem dosazeneho skore")
    plt.savefig("./{}" .format(name))
    plt.clf()

def gaussian_graph(scores, episodes_numbers, name):
    fig = plt.figure()

    score_gf = gaussian_filter(scores, sigma=7)
    
    plt.plot(episodes_numbers, score_gf)
    plt.ylabel("Skore")
    plt.xlabel("Epizody")
    plt.title("Graf uceni agenta (gaussuv filtr)")

    plt.savefig("./{}" .format(name))
    plt.clf()

def combined_graph(scores, episodes_numbers, name):
    fig = plt.figure()
    plt.plot(episodes_numbers, scores, 'ro')

    score_gf = gaussian_filter(scores, sigma=7)
    
    plt.plot(episodes_numbers, score_gf)
    plt.ylabel("Skore")
    plt.xlabel("Epizody")
    plt.title("Graf uceni agenta s dosazenym skore (gaussuv filtr)")

    plt.savefig("./{}" .format(name))
    plt.clf()

def learning_graph(env, agent, games, state_size, number_of_episodes, hop_lenght, path_to_enviroment_folder, weights_prefix):
    directories = next(os.walk("{}" .format(path_to_enviroment_folder)))[1]

    for directory in directories:
        print("{} - {}".format(path_to_enviroment_folder, directory))
        scores = []
        episodesList = []
        index = 0
        for eps in range(1000):
            avg_score = weights_test(env, agent, games, state_size, "{}/{}/{}{}.h5" .format(path_to_enviroment_folder, directory, weights_prefix, index))
            scores.append(avg_score)
            episodesList.append(index)
            index = index + hop_lenght
            if index == number_of_episodes:
                break
        
        line_graph(scores, episodesList)