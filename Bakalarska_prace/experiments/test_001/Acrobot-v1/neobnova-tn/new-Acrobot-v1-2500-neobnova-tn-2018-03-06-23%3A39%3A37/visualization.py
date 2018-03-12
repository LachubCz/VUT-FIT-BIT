import os
import copy
import matplotlib.pyplot as plt

def scattergrams_graph(scores, episodes_numbers, name):
    fig = plt.figure()
    plt.plot(episodes_numbers, scores, 'ro')
    plt.ylabel("Score")
    plt.xlabel("Episodes")
    plt.title("Vysledky")
    plt.savefig("./{}" .format(name))
    plt.clf()

def line_graph(scores, episodes_numbers, name):
    fig = plt.figure()
    plt.plot(episodes_numbers, scores)
    plt.ylabel("Score")
    plt.xlabel("Episodes")
    plt.title("Funkce interpolace vysledku")
    plt.savefig("./{}" .format(name))
    plt.clf()

def line_graph_i5(scores, episodes_numbers, name):
    score1 = copy.copy(scores)

    for i in range (len(scores)):
        if i > 1 and i < (len(scores)-2):
            score1[i] = (scores[i - 2] + scores[i - 1] + scores[i] + scores[i + 1] + scores[i + 2])/5

    fig = plt.figure()
    plt.plot(episodes_numbers, score1)
    plt.ylabel("Score")
    plt.xlabel("Episodes")
    plt.title("Funkce interpolace vysledku (filtr - prumer 5-ti prvku)")
    plt.savefig("./{}" .format(name))
    plt.clf()

def line_graph_i11(scores, episodes_numbers, name):
    score2 = copy.copy(scores)

    for i in range (len(scores)):
        if i > 4 and i < (len(scores)-5):
            score2[i] += scores[i - 5] + scores[i - 4] + scores[i - 3] + scores[i - 2] + scores[i - 1]
            score2[i] += scores[i + 5] + scores[i + 4] + scores[i + 3] + scores[i + 2] + scores[i + 1]
            score2[i] = score2[i]/11

    fig = plt.figure()
    plt.plot(episodes_numbers, score2)
    plt.ylabel("Score")
    plt.xlabel("Episodes")
    plt.title("Funkce interpolace vysledku  (filtr - prumer 11-cti prvku)")
    plt.savefig("./{}" .format(name))
    plt.clf()

def line_graph_i101(scores, episodes_numbers, name):
    score3 = copy.copy(scores)

    for i in range (len(scores)):
        if i > 49 and i < (len(scores) - 50):
            for e in range (1,50):
                score3[i] += scores[i - e] + scores[i + e] 
            score3[i] = score3[i]/101      

    fig = plt.figure()
    plt.plot(episodes_numbers, score3)
    plt.ylabel("Score")
    plt.xlabel("Episodes")
    plt.title("Funkce interpolace vysledku  (filtr - prumer 101 prvku)")
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