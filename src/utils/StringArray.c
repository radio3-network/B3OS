#include <stdlib.h>
#include <string.h>
#include "StringArray.h"

static StringArray* StringArray_create(int capacity) {
    StringArray *stringArray = (StringArray*) malloc(sizeof(StringArray));
    stringArray->strings = (const char**) malloc(capacity * sizeof(const char*));
    stringArray->size = 0;
    stringArray->capacity = capacity;
    return stringArray;
}

static void StringArray_destroy(StringArray *stringArray) {
    for (int i = 0; i < stringArray->size; i++) {
        free((char*) stringArray->strings[i]);
    }
    free(stringArray->strings);
    free(stringArray);
}

static int StringArray_add(StringArray *stringArray, const char *str) {
    if (stringArray->size == stringArray->capacity) {
        stringArray->capacity *= 2;
        stringArray->strings = (const char**) realloc(stringArray->strings, stringArray->capacity * sizeof(const char*));
    }
    char *newString = (char*) malloc(MAX_STR_LEN * sizeof(char));
    strcpy(newString, str);
    stringArray->strings[stringArray->size] = (const char *) newString;
    stringArray->size++;
    return (stringArray->size - 1);
}

static const char* StringArray_get(StringArray *stringArray, int index) {
    if (index < 0 || index >= stringArray->size) {
        return NULL;
    }
    return stringArray->strings[index];
}

static int StringArray_delete(StringArray *stringArray, int index) {
    if (index < 0 || index >= stringArray->size) {
        return 0;
    }
    free((char*) stringArray->strings[index]);
    for (int i = index; i < stringArray->size - 1; i++) {
        stringArray->strings[i] = stringArray->strings[i+1];
    }
    stringArray->size--;
    return 1;
}

static int StringArray_size(StringArray *stringArray) {
    return stringArray->size;
}
