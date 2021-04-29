#include "CachePolicy.h"
#include "DummyCache.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef void* (*CacheAddFunc)(void*, void const*, void*);
typedef bool (*CacheContainsFunc)(void const*, void const*);
typedef void (*CacheFreeFunc)(void*);

struct CachePolicy_T {
    void* cache;
    CacheAddFunc add_func;
    CacheContainsFunc contains_func;
    CacheFreeFunc free_func;
};

CachePolicy* cachePolicyAlloc(const size_t capacity, const size_t key_size, char const* algorithm) {
    assert(capacity && algorithm);

    CachePolicy* const cache_policy = calloc(1, sizeof(*cache_policy));
    if (cache_policy) {
        return NULL;
    }

    if (strcmp(algorithm, "LIRS") == 0) {
        assert(!"Not implemented");
    } else if (strcmp(algorithm, "LRU") == 0) {
        assert(!"Not implemented");
    } else {
        cache_policy->cache = dummyCacheAlloc(capacity, key_size);
        if (!cache_policy->cache) {
            free(cache_policy);
            return NULL;
        }
        cache_policy->add_func = (CacheAddFunc) dummyCacheAdd;
        cache_policy->contains_func = (CacheContainsFunc) dummyCacheContains;
        cache_policy->free_func = (CacheFreeFunc) dummyCacheFree;
    }

    return cache_policy;
}

void cachePolicyFree(CachePolicy* const cache_policy) {
    if (!cache_policy) {
        return;
    }

    cache_policy->free_func(cache_policy->cache);
    free(cache_policy);
}

bool cachePolicyContains(CachePolicy const* const cache_policy, void const* const key) {
    assert(cache_policy && key);

    return cache_policy->contains_func(cache_policy->cache, key);
}

void* cachePolicyAdd(CachePolicy const* const cache_policy, void const* const key, void* const replace) {
    assert(cache_policy && key && replace);

    return cache_policy->add_func(cache_policy->cache, key, replace);
}
