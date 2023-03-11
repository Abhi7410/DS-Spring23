import subprocess
import sys

jar_file = sys.argv[1] 

input_file = sys.argv[2] # input.txt

input_dir = sys.argv[3] # q2/input/

output_dir = sys.argv[4] # q2/output/

mapper_file = sys.argv[5] # 


# delete output directory if it exists
try:
    subprocess.call(["hadoop", "fs", "-rm", "-r", output_dir])
except:
    pass

try:
    subprocess.call(["hadoop", "fs", "-rm", input_dir + "*"])
except:
    pass



# edit input file to be in the format required by mapper
with open(input_file, "r") as f_in:
    numTimes = int(f_in.readline())

with open(input_file, "w") as f_out:
    for i in range(numTimes):
        f_out.write(str(i) + "\n")

subprocess.call(['hadoop', 'fs', '-copyFromLocal', input_file, input_dir])



# run mapper and reducer with python 

subprocess.call(['hadoop', 'jar', jar_file, '-file', mapper_file + '/mapper.py', '-mapper', 'python3 mapper.py' , '-file',
                mapper_file +'/reducer.py', '-reducer', 'python3 reducer.py', '-input', input_dir, '-output', output_dir])
