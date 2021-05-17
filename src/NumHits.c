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

int main() {
  size_t cache_capacity = 0;
  if (scanf("%zu", &cache_capacity) != 1) {
    return -1;
  }
  size_t size = 0;
  uint64_t *const arr = readArr(stdin, &size);
  if (!arr) {
    return -1;
  }

  const size_t num_misses = numMisses(arr, size, cache_capacity, "LIRS");
  const size_t num_hits = size - num_misses;
  printf("%zu\n", num_hits);

  free(arr);

  return 0;
}
