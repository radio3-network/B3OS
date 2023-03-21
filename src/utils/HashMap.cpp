#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

Usage examples:

HashMap* map = HashMapCreate();

    // Add items to the map
    HashMapInsert(map, "apple", (void*)"A fruit");

    // Get values from the map
    printf("%s\n", (char*)get(map, "apple"));  // Output: A fruit
    printf("%s\n", (char*)get(map, "banana"));  // Output: A fruit
    printf("%s\n", (char*)get(map, "carrot"));  // Output: A vegetable

    // Remove items from the map
    removeKey(map, "banana");

    // Check if an item exists
    printf("%d\n", containsKey(map, "apple"));  // Output: 1
    printf("%d\n", containsKey(map, "banana"));  // Output: 0

    // Iterate over all items in the map
    iterateHashMap(map);

    // Clear and destroy the map
    clearHashMap(map);
    destroyHashMap(map);

*/


// change the size as needed
#define HASHSIZE 101

typedef struct hashentry {
    const char* key;
    void* value;
    struct hashentry* next;
} HashEntry;

typedef struct hashmap {
    HashEntry* table[HASHSIZE];
} HashMap;

static unsigned hash(const char* key) {
    unsigned hashval = 0;
    for (hashval = 0; *key != '\0'; key++) {
        hashval = *key + 31 * hashval;
    }
    return hashval % HASHSIZE;
}

static HashMap* HashMapCreate() {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    memset(map->table, 0, sizeof(map->table));
    return map;
}

static void HashMapInsert(HashMap* map, const char* key, void* value) {
    HashEntry* entry = (HashEntry*)malloc(sizeof(HashEntry));
    entry->key = key;
    entry->value = value;
    unsigned hashval = hash(key);
    entry->next = map->table[hashval];
    map->table[hashval] = entry;
}

static void* HashMapGet(HashMap* map, const char* key) {
    HashEntry* entry;
    for (entry = map->table[hash(key)]; entry != NULL; entry = entry->next) {
        if (strcmp(key, entry->key) == 0) {
            return entry->value;
        }
    }
    return NULL;
}

static void HashMapRemoveKey(HashMap* map, const char* key) {
    unsigned hashval = hash(key);
    HashEntry* prev = NULL;
    HashEntry* entry = map->table[hashval];
    while (entry != NULL && strcmp(key, entry->key) != 0) {
        prev = entry;
        entry = entry->next;
    }
    if (entry == NULL) {
        return;
    }
    if (prev == NULL) {
        map->table[hashval] = entry->next;
    } else {
        prev->next = entry->next;
    }
    free(entry);
}

static int HashMapContainsKey(HashMap* map, const char* key) {
    unsigned hashval = hash(key);
    HashEntry* entry;
    for (entry = map->table[hashval]; entry != NULL; entry = entry->next) {
        if (strcmp(key, entry->key) == 0) {
            return 1;
        }
    }
    return 0;
}

static void HashMapClear(HashMap* map) {
    HashEntry* entry;
    HashEntry* temp;
    for (int i = 0; i < HASHSIZE; i++) {
        entry = map->table[i];
        while (entry != NULL) {
            temp = entry;
            entry = entry->next;
            free(temp);
        }
        map->table[i] = NULL;
    }
}

static void HashMapDestroy(HashMap* map) {
    HashMapClear(map);
    free(map);
}

static void HashMapIterate(HashMap* map) {
    HashEntry* entry;
    for (int i = 0; i < HASHSIZE; i++) {
        for (entry = map->table[i]; entry != NULL; entry = entry->next) {
            printf("Key: %s, Value: %p\n", entry->key, entry->value);
        }
    }
}
