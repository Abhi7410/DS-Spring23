import subprocess
import sys 


jar_file = sys.argv[1]

input_file = sys.argv[2]

input_dir = sys.argv[3]

output_dir = sys.argv[4]

mapper_file = sys.argv[5]


try:
    subprocess.call(["hadoop", "fs", "-rm", "-r", output_dir])
except:
    pass


try:
    subprocess.call(["hadoop", "fs", "-rm", input_dir + "*"])
except:
    pass


with open(input_file, "r") as f_in:

    m, n = map(int, f_in.readline().split())
    matrix_a = []

    for i in range(m):
        row = list(map(int, f_in.readline().split()))
        matrix_a.append(row)

    n, p = map(int, f_in.readline().split())

    matrix_b = []

    for i in range(n):
        row = list(map(int, f_in.readline().split()))
        matrix_b.append(row)


with open(input_file, "w") as f_out:

    for i in range(m):
        for j in range(n):
            pval = "A" + "," + str(i) + "," + str(j) + \
                "," + str(matrix_a[i][j]) + "," + str(p)
            f_out.write(pval + "\n")

    for i in range(n):
        for j in range(p):
            pval = "B" + "," + str(i) + "," + str(j) + \
                "," + str(matrix_b[i][j]) + "," + str(m)
            f_out.write(pval + "\n")


subprocess.call(['hadoop', 'fs', '-copyFromLocal', input_file, input_dir])

# run hadoop streaming
subprocess.call(['hadoop', 'jar', jar_file, '-file', mapper_file + '/mapper.py', '-mapper', 'python3 mapper.py', '-file',
                mapper_file + '/reducer.py', '-reducer', 'python3 reducer.py', '-input', input_dir, '-output', output_dir])




























