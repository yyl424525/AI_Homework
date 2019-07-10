from __future__ import print_function
from random import shuffle
import sys

n = sys.argv[1]
n = int(n)
puzzle = range(n * n)
shuffle(puzzle)
f = open('npuzzle_in.txt', 'w')
f.write(str(n) + "\n")
for i in range(n):
        for j in range(n):
                f.write(str(puzzle[i*n+j]) + ' ')
        f.write('\n')
shuffle(puzzle)
for i in range(n):
        for j in range(n):
                f.write(str(puzzle[i*n+j]) + ' ')
        f.write('\n')
f.close()
        
