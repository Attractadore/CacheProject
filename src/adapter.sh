#!/bin/bash

inp=$(</dev/stdin)
cache_size=$(echo $inp | cut -f 1 --delimiter=" ")
inp=$(echo $inp | cut -f 2- --delimiter=" ")
echo $inp | ./BenchCache LIRS $cache_size
