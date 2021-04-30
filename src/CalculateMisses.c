#include "CalculateMisses.h"

#define CACHE_POLICY_TYPE uint64_t
#define CACHE_POLICY_TYPE_NAME Int
#include <DataStructures/CachePolicy.h>
#define OHT_KEY_TYPE uint64_t
#define OHT_KEY_TYPE_NAME Int
#define OHT_VALUE_TYPE uint64_t
#define OHT_VALUE_TYPE_NAME
#include <DataStructures/OpenHashTable.h>

#include <assert.h>
#include <string.h>

static uint64_t intHash(uint64_t const* const ptr) {
    assert(ptr);

    return *ptr;
}

static bool intCompare(uint64_t const* const left, uint64_t const* const right) {
    assert(left && right);

    return *left == *right;
}

static const uint64_t dummyInt = (uint64_t) -1;

static uint64_t findRemoveElem(uint64_t const* const arr, const size_t len, OHTInt* const cache, const size_t cache_capacity, uint64_t remove_elem) {
    assert(arr);

    if (ohtIntSize(cache) < cache_capacity) {
        return dummyInt;
    }

    for (size_t i = 0; i < len; i++) {
        uint64_t const* find_ptr = ohtIntFind(cache, arr[i]);
        if (find_ptr) {
            remove_elem = *find_ptr;
        }
    }

    return remove_elem;
}

static size_t minNumMisses(uint64_t const* const arr, const size_t len, const size_t cache_capacity) {
    assert(arr && cache_capacity);

    OHTInt* const cache = ohtIntAlloc(intHash, intCompare);
    if (!cache) {
        return dummyInt;
    }

    size_t prev = dummyInt;
    size_t num_misses = 0;
    for (size_t i = 0; i < len; i++) {
        if (!ohtIntFind(cache, arr[i])) {
            num_misses++;
            const uint64_t remove_elem = findRemoveElem(arr + 1, len - i - 1, cache, cache_capacity, prev);
            ohtIntDelete(cache, remove_elem);
            ohtIntInsert(cache, arr[i], 0);
        }
        prev = arr[i];
    }

    ohtIntFree(cache);

    return num_misses;
}

size_t numMisses(uint64_t const* const arr, const size_t len, const size_t cache_capacity, char const* const cache_algorithm) {
    assert(arr && cache_capacity && cache_algorithm);

    if (strcmp(cache_algorithm, "OPT") == 0) {
        return minNumMisses(arr, len, cache_capacity);
    }

    CachePolicyInt* const cache_policy = cachePolicyIntAlloc(cache_capacity, intHash, intCompare, cache_algorithm);
    if (!cache_policy) {
        return dummyInt;
    }

    size_t num_misses = 0;
    for (size_t i = 0; i < len; i++) {
        num_misses += !cachePolicyIntContains(cache_policy, arr[i]);
        uint64_t replace = 0;
        if (cachePolicyIntAdd(cache_policy, arr[i], &replace) == CACHE_POLICY_ADD_ERROR) {
            cachePolicyIntFree(cache_policy);
            return dummyInt;
        }
    }

    cachePolicyIntFree(cache_policy);

    return num_misses;
}
