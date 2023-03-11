import sys

matrix_A = []
matrix_B = []
prev_index = None

for line in sys.stdin:
    line = line.strip()
    curr_index, value = line.split('\t')
    n,m = curr_index.split(',')
    matrix_name, index, val,p = value.split(',')
    p = int(p)
    index = int(index)
    val = int(val)
    if curr_index == prev_index:
        if matrix_name == 'A':
            matrix_A.append((index, val))
        else:
            matrix_B.append((index, val))
    else:
        if prev_index:
            res = 0
            if len(matrix_A) < len(matrix_B):
                matrix_A, matrix_B = matrix_B, matrix_A
            for i, valA in matrix_A:
                try:
                    valB = dict(matrix_B)[i]
                    res += valA * valB
                except KeyError:
                    pass
            print(str(res), end=' ')

            if int(prev_index.split(',')[1]) == p-1:
                print(" ")
        if matrix_name == 'A':
            matrix_A = [(index, val)]
            matrix_B = []
        else:
            matrix_B = [(index, val)]
            matrix_A = []
        prev_index = curr_index
if prev_index:
    res = 0
    if len(matrix_A) < len(matrix_B):
        matrix_A, matrix_B = matrix_B, matrix_A
    for i, valA in matrix_A:
        try:
            valB = dict(matrix_B)[i]
            res += valA * valB
        except KeyError:
            pass

    print(str(res), end=' ')
    if int(prev_index.split(',')[1]) == p-1:
        print(" ")















