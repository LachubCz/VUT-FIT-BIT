"""
file contains methods for visualization of learning progress
"""
import os
import os.path
import re
import warnings
import numpy as np
import matplotlib
matplotlib.use('Agg')
warnings.simplefilter('ignore', np.RankWarning)
warnings.simplefilter('ignore', RuntimeWarning)
import matplotlib.pyplot as plt
from matplotlib.pyplot import cm
from scipy.ndimage.filters import gaussian_filter

SMALL_SIZE = 10
MEDIUM_SIZE = 12
BIGGER_SIZE = 14

plt.rc('font', size=BIGGER_SIZE)
plt.rc('axes', titlesize=BIGGER_SIZE)
plt.rc('axes', labelsize=MEDIUM_SIZE)
plt.rc('xtick', labelsize=SMALL_SIZE)
plt.rc('ytick', labelsize=SMALL_SIZE)
plt.rc('legend', fontsize=MEDIUM_SIZE)
plt.rc('figure', titlesize=BIGGER_SIZE)

def get_name(graph_name):
    """
    method creates name for pdf
    """
    if os.path.isfile(graph_name+".pdf"):
        i=1
        while True:
            if not os.path.isfile(graph_name+"("+str(i)+").pdf"):
                graph_name = graph_name+"("+str(i)+").pdf"
                break
            i+=1
    else:
        graph_name = graph_name + ".pdf"
        
    return graph_name

def combined_graph(scores, episode_numbers, name, coordinates=None, linears=None, scatter=False):
    """
    method prints point graph and
    interpolation graph with gaussian filter of learning progress
    """
    if linears is not None:
        for key, value in linears.items():
            plt.plot([0, episode_numbers[-1]], [key, value], 'k-', linewidth=0.8)

    if scatter:
        plt.plot(episode_numbers, scores, 'ro', markersize=1)

    score_gf = gaussian_filter(scores, sigma=0.01791*episode_numbers[-1])

    plt.plot(episode_numbers, score_gf, linewidth=1)

    plt.ylabel("Skóre")
    plt.xlabel("Epizody")

    plt.xlim([0,coordinates[0]])
    if min(scores) < 0:
        plt.ylim([min(scores),coordinates[1]])
    else:
        plt.ylim([0,coordinates[1]])

    name = get_name(name)

    plt.savefig("./{}" .format(name), bbox_inches='tight')
    plt.clf()
    print("[Graph of learning progress visualization was made.]")

def heat_map(array, graph_name, axes):
    """
    method creates heatmap
    """
    fig, ax = plt.subplots()
    im = ax.imshow(array, cmap=cm.YlOrRd)
    if not axes:
        im.axes.get_yaxis().set_visible(False)
        im.axes.get_xaxis().set_visible(False)
    fig.tight_layout()
    
    graph_name = get_name(graph_name)
    plt.savefig("./{}" .format(graph_name), bbox_inches='tight')
    print("[Heatmap was made.]")

def split_data(line):
    """
    method splits varibles on line
    """
    data = list()
    arr = np.array([string for string in line.split(", ")], dtype=str)

    for i, item in enumerate(arr):
        word_parse = re.compile(r'''  ((?<=:.)-*[0-9]+\.*[0-9]*)''', re.X)
        parts = word_parse.findall(item)

        if parts != []:
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
    episode_numbers = list()
    counter = 0

    file = open(os.getcwd()+ "/" + filename, "r")

    while True:
        record = file.readline()
        if record == "":
            return data, episode_numbers

        processed = split_data(record)
        if processed:
            data.append(processed)
            episode_numbers.append(counter)
            counter=counter + 1

def get_visualization(filename, graph_name, idx_val, coordinates=None, linears=None, scatter=False):
    """
    method reads and proceses file with learning log
    """
    values = list()
    data, counter = read_file(filename)

    for i, item in enumerate(data):
        values.append(item[idx_val])

    if coordinates == None:
        combined_graph(values, counter, graph_name, [counter[-1], max(values)+10], linears, scatter)
    else:
        combined_graph(values, counter, graph_name, coordinates, linears, scatter)


if __name__ == "__main__":
    get_visualization("result.txt", "results", 6, coordinates=None, linears={22:22}, scatter=True)
