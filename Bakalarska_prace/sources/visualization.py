"""
methods for visualization of learning progress
"""
import os
import re
import warnings
import numpy as np
import matplotlib
matplotlib.use('Agg')
warnings.simplefilter('ignore', np.RankWarning)
warnings.simplefilter('ignore', RuntimeWarning)
import matplotlib.pyplot as plt
from scipy.ndimage.filters import gaussian_filter

def combined_graph(scores, episodes_numbers, name, coordinates, avg_value=None):
    """
    method prints point graph
    interpolation graph with gaussian filter of learning progress
    polynomial graph of learning progress
    """
    plt.plot(episodes_numbers, scores, 'ro', markersize=1)

    score_gf = gaussian_filter(scores, sigma=7)

    plt.plot(episodes_numbers, score_gf, linewidth=0.8)

    if avg_value is not None:
        plt.plot([0, episodes_numbers[-1]], [avg_value, avg_value], 'k-', linewidth=1)

    z = np.polyfit(episodes_numbers, scores, 50)
    f = np.poly1d(z)

    x_new = np.linspace(episodes_numbers[0], episodes_numbers[-1], 1000)
    y_new = f(x_new)

    plt.plot(x_new, y_new, 'lime', linewidth=1)

    plt.ylabel("Skóre")
    plt.xlabel("Epizody")

    plt.xlim([0,coordinates[0]])
    plt.ylim([0,coordinates[1]])
    plt.savefig("./{}" .format(name))
    plt.clf()

def split_data(line):
    """
    docstring
    """
    data = list()
    arr = np.array([string for string in line.split(", ")], dtype=str)

    for i, item in enumerate(arr):
        word_parse = re.compile(r'''  ((?<=:.)[0-9]+\.*[0-9]*)''', re.X)
        parts = word_parse.findall(item)
        if parts:
            data.append(float(parts[0]))

    if len(data) > 1:
        return data
    else:
        return []

def read_file(filename):
    """
    method opens file, processes it line by line and returns data with episode numbers
    """
    data = list()
    episodes_numbers = list()
    counter = 0

    file = open(os.getcwd()+ "/" + filename, "r")

    while True:
        record = file.readline()
        if record == "":
            return data, episodes_numbers

        processed = split_data(record)
        if processed:
            data.append(processed)
            episodes_numbers.append(counter)
            counter=counter + 1

def get_visualization(filename, graph_name, idx_val, coordinates, avg_value=None):
    """
    docstring
    """
    values = list()
    data, counter = read_file(filename)

    for i, item in enumerate(data):
        values.append(item[idx_val])

    combined_graph(values, counter, graph_name, coordinates, avg_value)

#get_visualization("results1.out", "max_value.pdf", 7, [72000, 6000], 1011)
