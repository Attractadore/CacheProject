#!/usr/bin/python3
import random

def main():
    n = int(input())
    print(n)
    j = 0
    l = 0
    for i in range(n):
        if j == l:
            j = 0
            l = random.randint(1, n // 4)
        print(j)
        j += 1

if __name__ == "__main__":
    main()
