import math
import os
import argparse

import matplotlib.pyplot as plt
import numpy as np


def read(source_dir):
    legend = []
    data = []
    for name in os.listdir(source_dir):
        if not name.endswith("_collision_time.csv") or not name.startswith("Helbing"):
            continue

        my_data = np.genfromtxt(source_dir + name, delimiter=',')
        legend.append(name)
        data.append(np.array(my_data[1:, 1]))

    return legend, data


def hist(xs, bins, range):
    h = np.zeros((bins + 2,), dtype=np.int)
    m, M = range
    bin_size = (M - m) / bins
    for x in xs:
        if x < m:
            h[0] += 1
            continue
        if x > M:
            h[-1] += 1
            continue

        idx = math.floor((x - m) / bin_size)
        h[idx + 1] += 1

    return h


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("data_dir")
    args = parser.parse_args()
    data_dir = args.data_dir

    names, data = read(data_dir)
    fig, axs = plt.subplots(1, 1)

    range1 = (0.0, 1.0)
    bins = 30

    axs.boxplot(data, showfliers=False)
    #for d in data:
    #    x = sorted(d)
    #    axs.plot(x)

    #for d in data:
    #    x = hist(d, bins, range1)
    #    axs.plot(np.linspace(*range1, num=bins + 2), x, label=names)

    #axs.set_ylim([0, 500])
    axs.legend(names)
    #plt.xticks(ticks=list(range(len(names))), labels=names, rotation="vertical")
    plt.ylabel("Time agent spent in collisions, s")

    plt.show()
