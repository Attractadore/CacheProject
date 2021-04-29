#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct CachePolicy_T CachePolicy;

CachePolicy* cachePolicyAlloc(size_t capacity, size_t key_size, char const* algorithm);
bool cachePolicyContains(CachePolicy const* const cache_policy, void const* key);
void* cachePolicyAdd(CachePolicy const* const cache_policy, void const* key, void* replace);
void cachePolicyFree(CachePolicy* cache_policy);
