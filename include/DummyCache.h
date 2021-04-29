#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct DummyCache_T DummyCache;

DummyCache* dummyCacheAlloc(size_t capacity, size_t key_size);
void dummyCacheFree(DummyCache* cache);

void* dummyCacheAdd(DummyCache* cache, void const* key, void* replace);

bool dummyCacheContains(DummyCache const* cache, void const* key);
