import numpy as np

def read_matrix(file):
    with open(file, 'r') as f:
        n = int(f.readline())
        m = []
        for i in range(n):
            row = list(map(int, f.readline().split()))
            m.append(row)
    return np.array(m)

A = read_matrix("C:\\Users\\user\\Desktop\\учеба\\A.txt")
B = read_matrix("C:\\Users\\user\\Desktop\\учеба\\B.txt")
C = read_matrix("C:\\Users\\user\\Desktop\\учеба\\C.txt")

C_true = A @ B

print("C from C++:")
print(C)
print("C from Python:")
print(C_true)

if np.array_equal(C, C_true):
    print("Verification OK")
else:
    print("Verification FAILED")
