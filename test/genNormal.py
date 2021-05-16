#!/usr/bin/python3
"""
Generate a sequence whose values are normally distributed
"""
import random

def main():
    n = int(input())
    print(n)
    for i in range(n):
        print(max(int(random.gauss(n, n / 4)), 0))

if __name__ == "__main__":
    main()
