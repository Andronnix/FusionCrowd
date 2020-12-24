import os
import numpy as np
import matplotlib.pyplot as plt
import argparse


def read(source_dir, exclude_names=None):
    if exclude_names is None:
        exclude_names = lambda x: False

    legend = []
    data = []
    for name in os.listdir(source_dir):
        if not name.endswith("_trajs.csv") or exclude_names(name):
            continue

        my_data = []
        fullname = source_dir + name

        print("Reading {}".format(name))
        prev = 0.0
        with open(fullname, 'r') as fin:
            for line in fin.readlines():
                if len(line.strip()) == 0:
                    continue

                time, *_ = line.split(",", 1)
                my_data.append(float(time) - prev)
                prev = float(time)

        legend.append(name)
        data.append(np.array(my_data))

    return legend, data


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("data_dir")
    args = parser.parse_args()
    data_dir = args.data_dir

    names, data = read(data_dir, lambda n: not n.startswith("Helbing"))

    fig, axs = plt.subplots(1, 1)

    bins = np.linspace(0, 0.1, 8)
    ticks = (bins[:-1] + bins[1:]) / 2
    tick_labels = list("[{:.2}, {:.2}]".format(f, t) for f, t in zip(bins[:-1], bins[1:]))

    axs.hist(data, bins=bins)

    #for d in data:
    #    axs.plot(d, marker='.')

    #axs.boxplot(data)
    #for d in data:
    #    axs.plot(d[2:])

    axs.legend(names)
    plt.title("Step time histogram, tradeshow2")
    plt.ylabel("# of steps")
    plt.xlabel("Step time, s")

    #plt.xticks(ticks, tick_labels, rotation=20)

    plt.show()
