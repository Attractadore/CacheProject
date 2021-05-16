#!/usr/bin/python3
"""
Generate a sequence where each item is likely to occur again in the near future
"""
import random

def main():
    n = int(input())
    print(n)
    data = [random.randint(1, n // 4) for i in range(n)]
    weights = [i * i for i in range(1, n + 1)]
    for i in range(n):
        data.append(random.choices(data, cum_weights=weights)[0])
        v = n + i + 1
        weights.append(v * v)
        print(data[-1])

if __name__ == "__main__":
    main()
