#include "CalculateMisses.h"

#define CACHE_POLICY_TYPE uint64_t
#define CACHE_POLICY_TYPE_NAME Int
#include <DataStructures/CachePolicy.h>
#define CHT_KEY_TYPE uint64_t
#define CHT_KEY_TYPE_NAME Int
#define CHT_VALUE_TYPE size_t
#define CHT_VALUE_TYPE_NAME
#include <DataStructures/ChainHashTable.h>

typedef struct {
  uint64_t value;
  size_t index;
} ValueIndexPair;

#define BIN_HEAP_TYPE ValueIndexPair
#define BIN_HEAP_TYPE_NAME
#include <DataStructures/BinHeap.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static uint64_t intHash(uint64_t const *const ptr) {
  assert(ptr);

  return *ptr;
}

static bool intCompare(uint64_t const *const left,
                       uint64_t const *const right) {
  assert(left && right);

  return *left == *right;
}

int valueIndexPairCompare(ValueIndexPair const *const left,
                          ValueIndexPair const *const right) {
  assert(left && right);

  if (left->index < right->index) {
    return -1;
  }

  if (left->index > right->index) {
    return 1;
  }

  return 0;
}

static size_t *getForwardIndices(uint64_t const *const arr, const size_t len) {
  assert(arr);

  CHTInt *const prev_indices = chtIntAlloc(intHash, intCompare);
  if (!prev_indices) {
    return NULL;
  }

  size_t *const forward_indices = calloc(len, sizeof(*forward_indices));
  if (!forward_indices) {
    chtIntFree(prev_indices);
    return NULL;
  }

  for (size_t i = len - 1; i < len; i--) {
    uint64_t *const ptr = chtIntFind(prev_indices, arr[i]);
    const size_t prev_index = (ptr) ? (*ptr) : (SIZE_MAX);
    forward_indices[i] = prev_index;
    if (!chtIntInsert(prev_indices, arr[i], i)) {
      chtIntFree(prev_indices);
      free(forward_indices);
      return NULL;
    }
  }

  chtIntFree(prev_indices);

  return forward_indices;
}

static size_t minNumMisses(uint64_t const *const arr, const size_t len,
                           const size_t cache_capacity) {
  assert(arr && cache_capacity);

  CHTInt *const cache = chtIntAlloc(intHash, intCompare);
  BinHeap *const queue = binHeapAlloc(valueIndexPairCompare);
  size_t *const forward_indices = getForwardIndices(arr, len);
  if (!cache || !queue || !forward_indices) {
    chtIntFree(cache);
    binHeapFree(queue);
    free(forward_indices);

    return SIZE_MAX;
  }

  size_t num_misses = 0;
  for (size_t i = 0; i < len; i++) {
    if (!chtIntFind(cache, arr[i])) {
      num_misses++;
      if (chtIntSize(cache) == cache_capacity) {
        const uint64_t max = binHeapPop(queue).value;
        chtIntDelete(cache, max);
      }
      assert(chtIntSize(cache) < cache_capacity);

      if (!chtIntInsert(cache, arr[i], 0)) {
        num_misses = SIZE_MAX;
        break;
      }
    }

    if (!binHeapInsert(queue, (ValueIndexPair){.value = arr[i],
                                               .index = forward_indices[i]})) {
      num_misses = SIZE_MAX;
      break;
    }
  }

  chtIntFree(cache);
  binHeapFree(queue);
  free(forward_indices);

  return num_misses;
}

size_t numMisses(uint64_t const *const arr, const size_t len,
                 const size_t cache_capacity,
                 char const *const cache_algorithm) {
  assert(arr && cache_capacity && cache_algorithm);

  if (strcmp(cache_algorithm, "OPT") == 0) {
    return minNumMisses(arr, len, cache_capacity);
  }

  CachePolicyInt *const cache_policy = cachePolicyIntAlloc(
      cache_capacity, intHash, intCompare, getCacheAlgorithm(cache_algorithm));
  if (!cache_policy) {
    return SIZE_MAX;
  }

  size_t num_misses = 0;
  for (size_t i = 0; i < len; i++) {
    num_misses += !cachePolicyIntContains(cache_policy, arr[i]);
    uint64_t replace = 0;
    if (cachePolicyIntAdd(cache_policy, arr[i], &replace) ==
        CACHE_POLICY_ADD_ERROR) {
      cachePolicyIntFree(cache_policy);
      return SIZE_MAX;
    }
  }

  cachePolicyIntFree(cache_policy);

  return num_misses;
}
