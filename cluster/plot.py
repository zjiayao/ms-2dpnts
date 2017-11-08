import numpy as np
import matplotlib.pyplot as plt
import os
import glob

FWD = os.path.dirname(os.path.realpath(__file__))
OUTPUT = os.path.join(FWD, 'oup')
MODES = os.path.join(FWD, 'ms_modes')


def plot_data(files, xlabel=r'$x_1$', ylabel=r'$x_2$', headers=None, seps=None):
	fig = plt.figure()
	for i, datafile in enumerate(files):
		X = []
		Y = []
		if os.path.exists(datafile):
			with open(datafile) as data:
				if headers is not None and headers[i]:
					data.readline()
				for line in data:
					x, y = line.split(sep=' ' if seps is None else seps[i])
					X.append(float(x))
					Y.append(float(y))
			plt.scatter(X, Y, linewidth=1)
	plt.xlabel(r'$x_1$', fontsize=16)
	plt.ylabel(r'$x_2$', fontsize=16)
	plt.legend()
	plt.show(block=0)

if __name__ == '__main__':
	plot_data([OUTPUT])
	plot_data([MODES])
	plot_data(glob.glob(os.path.join(FWD, 'cluster_[0-9]*')))





