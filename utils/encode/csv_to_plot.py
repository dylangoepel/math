import matplotlib.pyplot as plt
import csv
import sys

if len(sys.argv) != 2:
    sys.exit(1)

xs = []
ys = []
r = csv.reader(sys.stdin)
for line in r:
    xs.append(float(line[0]))
    ys.append(float(line[1]))
plt.plot(xs, ys)
plt.savefig(sys.argv[1])
