#include "CalculateMisses.h"

#include <DataStructures/BaseCachePolicy.h>

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

static uintptr_t *readArr(FILE *const file, size_t *const size_ptr) {
  assert(file && size_ptr);

  size_t num = 0;
  if (fscanf(file, "%zu", &num) != 1) {
    return NULL;
  }

  uint64_t *const arr = calloc(num, sizeof(*arr));
  if (!arr) {
    return NULL;
  }

  for (size_t i = 0; i < num; i++) {
    if (fscanf(file, "%" SCNu64, &arr[i]) != 1) {
      free(arr);
      return NULL;
    }
  }

  *size_ptr = num;

  return arr;
}

static void printCacheAlgorithms(FILE *const file) {
  assert(file);

  fprintf(stderr, "Available algorithms are: ");
  for (size_t i = 0; i < CACHE_ALGORITHM_INVALID; i++) {
    fprintf(stderr, "%s, ", cacheAlgorithmNames[i]);
  }
  fprintf(stderr, "OPT\n");
}

int main(int argc, char const *const *argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s [CachingAlgorithm] [CacheCapacity]\n", argv[0]);
    printCacheAlgorithms(stderr);
    return -1;
  }

  char const *const cache_algorithm = argv[1];
  size_t cache_capacity = 0;
  if (sscanf(argv[2], "%zu", &cache_capacity) != 1) {
    return -1;
  }

  size_t size = 0;
  uint64_t *const arr = readArr(stdin, &size);
  if (!arr) {
    return -1;
  }

  {
    const size_t num_misses =
        numMisses(arr, size, cache_capacity, cache_algorithm);
    printf("%zu\n", num_misses);
  }

  free(arr);

  return 0;
}
