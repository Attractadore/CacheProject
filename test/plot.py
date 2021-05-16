#!/usr/bin/python3
"""
Plot benchmark results
"""
import matplotlib.pyplot as plt
import numpy as np
from sys import argv

def getfrom_file(name):
    data = np.loadtxt(name)
    size = data[:,0]
    cachemiss = data[:,1]
    return size, cachemiss

def main():
    if len(argv) <= 3 or (len(argv) - 3) % 2 != 0:
        print("Usage: {0} [chart_title] [save_file] [[plot_label] [plot_file]]".format(argv[0]))
        return

    plt.title(argv[1])
    plt.xlabel("Cache size")
    plt.ylabel("Cache misses")
    for i in range(4, len(argv), 2):
        size, cachemiss = getfrom_file(argv[i])
        plt.plot(size, cachemiss)
    plt.legend(argv[3::2])
    plt.savefig(argv[2])

if __name__ == "__main__":
    main();
