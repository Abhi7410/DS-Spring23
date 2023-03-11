import sys

total_counts = {}

for line in sys.stdin:
    v, count = line.strip().split('\t')
    count = int(count)

    if v not in total_counts:
        total_counts[v] = 0
    total_counts[v] += count

sum_V = 0
num_samples = 0
for v in total_counts:
    sum_V += int(v) * total_counts[v]
    num_samples += total_counts[v]
expected_V = float(sum_V) / num_samples


print("Expected value of V is e: E(V) =  {}".format(expected_V))

# TC = O(n)