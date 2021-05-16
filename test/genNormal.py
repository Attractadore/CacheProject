#!/usr/bin/python3
import random

def main():
    n = int(input())
    print(n)
    for i in range(n):
        print(int(random.gauss(n, n / 4)))

if __name__ == "__main__":
    main()
