import sys
import random

counts = {}

for line in sys.stdin:
    val = int(line.strip())
    random.seed(val)
    key = 0
    sumX = 0
    while sumX < 1:
        x = random.uniform(0,1)
        sumX += x
        key += 1
    if key not in counts:
        counts[key] = 0
    counts[key] += 1

for key in counts:
    print(str(key) + '\t' + str(counts[key]))


# TC = O(ne)