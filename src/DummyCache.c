#include "DummyCache.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct DummyCache_T {
    size_t key_size;
    bool in_use;
    unsigned char data[];
};

DummyCache* dummyCacheAlloc(const size_t capacity, const size_t key_size) {
    DummyCache* const cache = calloc(sizeof(*cache) + key_size, sizeof(unsigned char));
    if (cache) {
        cache->key_size = key_size;
    }
    return cache;
}

void dummyCacheFree(DummyCache* const cache) {
    free(cache);
}

bool dummyCacheContains(DummyCache const* const cache, void const* const key) {
    assert(cache && key);

    if (!cache->in_use) {
        return false;
    }

    return memcmp(key, cache->data, cache->key_size);
}

void* dummyCacheAdd(DummyCache* const cache, void const* const key, void* const replace) {
    assert(cache && key && replace);

    if (cache->in_use) {
        memcpy(replace, cache->data, cache->key_size);
    }

    memcpy(cache->data, key, cache->key_size);

    if (cache->in_use) {
        return replace;
    }

    cache->in_use = true;

    return NULL;
}
