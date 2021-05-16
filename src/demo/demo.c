#define VECTOR_TYPE char
#define VECTOR_TYPE_NAME Char
#include <DataStructures/Vector.h>
#define VECTOR_TYPE char const *
#define VECTOR_TYPE_NAME ConstCharPtr
#include <DataStructures/Vector.h>
#define CACHE_POLICY_TYPE char const *
#define CACHE_POLICY_TYPE_NAME ConstCharPtr
#include <DataStructures/CachePolicy.h>

#include <curl/curl.h>

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t writeBuffer(char const *const file_name, char const *const buffer,
                   const size_t buffer_size) {
  assert(file_name && buffer && buffer_size);

  FILE *const file = fopen(file_name, "w");
  if (!file) {
    return 0;
  }

  const size_t write_size = fwrite(buffer, sizeof(*buffer), buffer_size, file);

  fclose(file);

  return write_size;
}

size_t writeToVector(char *const ptr, const size_t size, const size_t nmemb,
                     void *const userdata) {
  assert(ptr && userdata);

  VectorChar *const buffer = userdata;

  for (size_t i = 0; i < nmemb; i++) {
    if (!vectorCharPushBack(buffer, ptr[i])) {
      return i;
    }
  }

  return nmemb;
}

uint64_t stringHash(char const *const *const ptr) {
  assert(ptr);

  uint64_t hash = 0;
  char const *str = *ptr;
  while (*str) {
    hash = hash * 37 + (uint64_t) * (str++);
  }

  return hash;
}

bool stringCompare(char const *const *const left_ptr,
                   char const *const *const right_ptr) {
  assert(left_ptr && right_ptr);

  return strcmp(*left_ptr, *right_ptr) == 0;
}

bool visitAll(char const *const *const urls, const size_t num,
              const size_t cache_capacity,
              const CacheAlgorithm cache_algorithm) {
  assert(urls && cache_capacity && cache_algorithm);

  VectorChar *const vector = vectorCharAlloc();
  CachePolicyConstCharPtr *const cache_policy = cachePolicyConstCharPtrAlloc(
      cache_capacity, stringHash, stringCompare, cache_algorithm);
  CURL *const handle = curl_easy_init();
  if (!vector || !cache_policy || !handle) {
    goto cleanup;
  }

  if (curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, true)) {
    goto cleanup;
  }
  if (curl_easy_setopt(handle, CURLOPT_WRITEDATA, vector)) {
    goto cleanup;
  }
  if (curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writeToVector)) {
    goto cleanup;
  }

  for (size_t i = 0; i < num; i++) {
    printf("Visit %s...\n", urls[i]);
    if (cachePolicyConstCharPtrContains(cache_policy, urls[i])) {
      printf("Retrieve %s from cache\n", urls[i]);
      continue;
    }

    if (curl_easy_setopt(handle, CURLOPT_URL, urls[i])) {
      goto cleanup;
    }
    if (curl_easy_perform(handle)) {
      goto cleanup;
    }

    const size_t buffer_size = vectorCharSize(vector);
    if (writeBuffer(urls[i], vectorCharConstData(vector), buffer_size) !=
        buffer_size) {
      goto cleanup;
    }

    printf("Add %s to cache\n", urls[i]);
    char const *replace = NULL;
    switch (cachePolicyConstCharPtrAdd(cache_policy, urls[i], &replace)) {
    case CACHE_POLICY_ADD_ERROR:
      goto cleanup;
    case CACHE_POLICY_ADD_REPLACE:
      printf("Remove %s from cache\n", replace);
      remove(replace);
    case CACHE_POLICY_ADD_NO_REPLACE:
      break;
    default:
      assert(!"Invalid enumration value");
    }

    vectorCharClear(vector);
  }

  curl_easy_cleanup(handle);
  cachePolicyConstCharPtrFree(cache_policy);
  vectorCharFree(vector);

  return true;

cleanup:
  curl_easy_cleanup(handle);
  cachePolicyConstCharPtrFree(cache_policy);
  vectorCharFree(vector);

  return false;
}

char *readString(FILE *const file) {
  assert(file);

  VectorChar *const buffer = vectorCharAlloc();
  if (!buffer) {
    return NULL;
  }

  int c = '\0';
  while ((c = fgetc(file)) != EOF) {
    if (!vectorCharPushBack(buffer, c)) {
      vectorCharFree(buffer);
      return NULL;
    }
  }

  if (!vectorCharPushBack(buffer, '\0')) {
    vectorCharFree(buffer);
    return NULL;
  }

  char *const buffer_data = vectorCharDisown(buffer, NULL, NULL);
  vectorCharFree(buffer);

  return buffer_data;
}

char const **getUrls(char *str, size_t *const num_ptr) {
  assert(str && num_ptr);

  VectorConstCharPtr *const urls = vectorConstCharPtrAlloc();
  if (!urls) {
    return NULL;
  }

  while (*str) {
    while (*str && isspace(*str)) {
      *(str++) = '\0';
    }

    if (!*str) {
      break;
    }

    if (!vectorConstCharPtrPushBack(urls, str)) {
      vectorConstCharPtrFree(urls);
      return NULL;
    }

    while (*str && !isspace(*str)) {
      str++;
    }
  }

  char const **const urls_data = vectorConstCharPtrDisown(urls, num_ptr, NULL);
  vectorConstCharPtrFree(urls);

  return urls_data;
}

void printStrings(FILE *const file, char const *const *const strs,
                  const size_t num) {
  assert(strs);

  for (size_t i = 0; i < num; i++) {
    fprintf(file, "%s\n", strs[i]);
  }
}

int main(int argc, char const *const *const argv) {
  char *const str = readString(stdin);
  if (!str) {
    return -1;
  }

  size_t num_urls = 0;
  char const **urls = getUrls(str, &num_urls);
  if (!urls) {
    free(str);
    return -1;
  }

  const size_t cache_capacity = 128;
  const CacheAlgorithm cache_algorithm =
      (argc > 1) ? (getCacheAlgorithm(argv[1])) : (CACHE_ALGORITHM_DUMMY);
  visitAll(urls, num_urls, cache_capacity, cache_algorithm);

  free(urls);
  free(str);

  return 0;
}
