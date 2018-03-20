"""
methods for learning progress vizualization
"""
import matplotlib.pyplot as plt
from scipy.ndimage.filters import gaussian_filter

def point_graph(scores, episodes_numbers, name):
    """
    method prints point graph of learning progress
    """
    plt.plot(episodes_numbers, scores, 'ro')
    plt.ylabel("Skore")
    plt.xlabel("Epizody")
    plt.title("Graf agentem dosazeneho skore")
    plt.savefig("./{}" .format(name))
    plt.clf()

def gaussian_graph(scores, episodes_numbers, name):
    """
    method prints interpolation graph with gaussian filter of learning progress
    """
    score_gf = gaussian_filter(scores, sigma=7)

    plt.plot(episodes_numbers, score_gf)
    plt.ylabel("Skore")
    plt.xlabel("Epizody")
    plt.title("Graf uceni agenta (gaussuv filtr)")

    plt.savefig("./{}" .format(name))
    plt.clf()

def combined_graph(scores, episodes_numbers, name):
    """
    method prints point graph and interpolation graph with gaussian filter of learning progress
    """
    plt.plot(episodes_numbers, scores, 'ro')

    score_gf = gaussian_filter(scores, sigma=7)

    plt.plot(episodes_numbers, score_gf)
    plt.ylabel("Skore")
    plt.xlabel("Epizody")
    plt.title("Graf uceni agenta s dosazenym skore (gaussuv filtr)")

    plt.savefig("./{}" .format(name))
    plt.clf()
