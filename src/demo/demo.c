#define VECTOR_TYPE char
#define VECTOR_TYPE_NAME Char
#include <DataStructures/Vector.h>
#define VECTOR_TYPE char const*
#define VECTOR_TYPE_NAME ConstCharPtr
#include <DataStructures/Vector.h>

#include <curl/curl.h>

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

size_t writeBuffer(char const* const file_name, char const* const buffer, const size_t buffer_size) {
    assert(file_name && buffer && buffer_size);

    FILE* const file = fopen(file_name, "w");
    if (!file) {
        return 0;
    }

    const size_t write_size = fwrite(buffer, sizeof(*buffer), buffer_size, file);

    fclose(file);

    return write_size;
}

size_t writeToVector(char* const ptr, const size_t size, const size_t nmemb, void* const userdata) {
    assert(ptr && userdata);

    VectorChar* const buffer = userdata;

    for (size_t i = 0; i < nmemb; i++) {
        if (!vectorCharPushBack(buffer, ptr[i])) {
            return i;
        }
    }

    return nmemb;
}

bool downloadAll(char const* const* const urls, const size_t num) {
    assert(urls);

    CURL* const handle = curl_easy_init();

    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, true);

    VectorChar* const vector = vectorCharAlloc();

    curl_easy_setopt(handle, CURLOPT_WRITEDATA, vector);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, writeToVector);

    for (size_t i = 0; i < num; i++) {
        curl_easy_setopt(handle, CURLOPT_URL, urls[i]);
        curl_easy_perform(handle);
        writeBuffer(urls[i], vectorCharConstData(vector), vectorCharGetSize(vector));
        vectorCharClear(vector);
    }

    curl_easy_cleanup(handle);

    return false;
}

char* readString(FILE* const file) {
    assert(file);

    VectorChar* const buffer = vectorCharAlloc();
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

    return vectorCharDisown(buffer, NULL, NULL);
}

char const** getUrls(char* str, size_t* const num_ptr) {
    assert(str && num_ptr);

    VectorConstCharPtr* const urls = vectorConstCharPtrAlloc();
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

    return vectorConstCharPtrDisown(urls, num_ptr, NULL);
}

void printStrings(FILE* const file, char const* const* const strs, const size_t num) {
    assert(strs);

    for (size_t i = 0; i < num; i++) {
        fprintf(file, "%s\n", strs[i]);
    }
}

int main() {
    char* const str = readString(stdin);
    if (!str) {
        return -1;
    }

    size_t num_urls = 0;
    char const** urls = getUrls(str, &num_urls);
    if (!urls) {
        free(str);
        return -1;
    }

    printStrings(stdout, urls, num_urls);
    downloadAll(urls, num_urls);

    free(urls);
    free(str);

    return 0;
}
