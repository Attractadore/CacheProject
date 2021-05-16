#pragma once

#include <stddef.h>
#include <stdint.h>

size_t numMisses(uint64_t const *arr, size_t len, size_t cache_capacity,
                 char const *cache_algorithm);
