#!/usr/bin/python3
"""
Generate a sequence whose values are uniformly distributed
"""
import random

def main():
    n = int(input())
    print(n)
    for i in range(n):
        print(random.randint(1, n // 4))

if __name__ == "__main__":
    main()
