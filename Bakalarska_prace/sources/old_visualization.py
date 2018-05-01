import os
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from scipy.ndimage.filters import gaussian_filter

def combined_graph(scores, episodes_numbers, name):
    """
    method prints point graph and interpolation graph with gaussian filter of learning progress
    """
    plt.plot(episodes_numbers, scores, 'ro', markersize=1)

    score_gf = gaussian_filter(scores, sigma=7)

    plt.plot(episodes_numbers, score_gf, linewidth=0.8)

    plt.plot([0, episodes_numbers[-1]], [1011, 1011], 'k-', linewidth=1)

    z = np.polyfit(episodes_numbers, scores, 50)
    f = np.poly1d(z)

    x_new = np.linspace(episodes_numbers[0], episodes_numbers[-1], 1000)
    y_new = f(x_new)

    plt.plot(x_new, y_new, 'lime', linewidth=1)


    plt.ylabel("Skóre")
    plt.xlabel("Epizody")
    plt.title("2048")
    plt.xlim([0,45000])
    plt.ylim([0,7000])
    plt.savefig("./{}" .format(name))
    plt.clf()

dataset = list()
numbers = list()
counter = 0
file = open(os.getcwd()+"/" + "results.out", "r")
while True:
    customer = file.readline()
    if customer == "":
        break
    dataset.append(float(customer))
    numbers.append(counter)
    counter=counter+1

combined_graph(dataset, numbers, "{}.pdf" .format("results"))