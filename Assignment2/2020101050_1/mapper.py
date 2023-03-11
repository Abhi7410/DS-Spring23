# matrix multiplication mapper
import sys

# nxm * mxp = nxp

for line in sys.stdin:
    line = line.strip()
    matrix_name, row, col, value,col_x = line.split(',')
    col_x = int(col_x)
    if matrix_name == 'A':
        p = col_x
        for k in range(0,col_x):
            key = str(row) + ',' + str(k)
            ans = key + '\t' + matrix_name + ',' + col + ',' + value + ',' + str(p)
            print(ans)
            
    else:
        for k in range(0,col_x):
            key = str(k) + ',' + str(col)
            ans = key + '\t' + matrix_name + ',' + row + ',' + value + ',' + str(p)
            print(ans)


# TC = (nxmxp)/num_mappers

