import os
import sys
from collections import defaultdict
import matplotlib.pyplot as plt
import numpy as np
import argparse
from matplotlib.ticker import FuncFormatter, FixedLocator
from matplotlib.transforms import Affine2D

# divided into pages
# {name -> (total, mean)}
# (control-flow benchmarks, data-flow benchmarks)
def load_data(input_path):
    c = {}
    d = {}
    page = 0
    with open(input_path, 'r') as f:
        data = f.read()
        lines = data.split('\n')
        for (idx,line) in enumerate(lines):
            if idx == 0:
                continue
            if line.startswith("---"):
                page += 1
                continue
            split_line = [l.strip() for l in line.split('|')]
            if page == 1:
                c[split_line[0]] = (int(split_line[1]), int(split_line[2]))
            if page == 2:
                d[split_line[0]] = (int(split_line[1]), int(split_line[2]))
    return (c,d)

def make_graph(all_times, output_path, title="", xlabel="", ylabel=""):
    fig = plt.figure()
    ax = fig.add_axes([0,0,1,1])
    benchnames = all_times.keys()
    times = [v[1] for v in all_times.values()] 
    ax.bar(benchnames, times)
    
    fig.suptitle(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    
    plt.savefig(output_path, format="pdf", bbox_inches="tight", pad_inches=0)

def run(input_path, output_path_base):
    c,d = load_data(input_path)
    make_graph(c, f"{output_path_base}.control_flow.pdf", 
        title="Cost of making domain transitions", 
        xlabel="# of args", 
        ylabel="Overhead (cycles)")
    make_graph(d, f"{output_path_base}.data_flow.pdf",
    title="Cost of transferring data between domains",
    xlabel="# of bytes",
    ylabel="Overhead (cycles)")

def main():
    parser = argparse.ArgumentParser(description='Graph Results')
    parser.add_argument('input_path', help='input directory containing results')
    parser.add_argument('-o', dest='output_path_base', help='output path for graphs')
    args = parser.parse_args()
    run(args.input_path, args.output_path_base)


if __name__ == '__main__':
    main()