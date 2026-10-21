#include <string.h>

#include "util/hash.h"
#include "util/int32_map.h"

void init_int32_map(int32_map_t *map, uint32_t size)
{
    map->capacity = size <= 0 ? DEFAULT_INT32_MAP_SIZE : size; 
    map->data = calloc(map->capacity, sizeof(int32_map_entry_t*));
}

void delete_int32_map(int32_map_t *map)
{
    int32_map_reset(map);
    free(map->data);
}

void int32_map_reset(int32_map_t *map)
{
    uint32_t i;
    int32_map_entry_t *cur, *next;

    for (i = 0; i < map->capacity; ++i) {
        cur = map->data[i];
        while (cur != NULL) {
            next = cur->next;
            free(cur);
            cur = next;
        }
        map->data[i] = NULL;
    }
}

/**
 * Returns the value for `key` if `key` is present in `map`. Returns -1 if
 * nothing is present.
 */
int32_t int32_map_find(int32_map_t *map, int32_t key)
{
    uint32_t hash = jenkins_hash_uint32((int32_t) key) & (map->capacity - 1);
    
    if (map->data[hash] == NULL) {
        return -1;
    }

    int32_map_entry_t *cur;
    for (cur = map->data[hash]; cur != NULL; cur = cur->next) {
        if (cur->key == key) {
            return cur->value;
        }
    }
    return -1;
}

/**
 * Adds `value` to `map` with key `key`.
 */
void int32_map_add(int32_map_t *map, int32_t key, int32_t value)
{
    int32_map_entry_t *entry;
    entry = malloc(sizeof(int32_map_entry_t));
    entry->key = key;
    entry->value = value;
    entry->next = NULL;

    uint32_t hash = jenkins_hash_uint32((int32_t) key) & (map->capacity - 1);

    if (map->data[hash] == NULL) {
        map->data[hash] = entry;
        return;
    }
    
    int32_map_entry_t *cur;
    for (cur = map->data[hash]; cur->next != NULL; cur = cur->next)
        ;
    cur->next = entry;
}

/**
 * Returns the value for `key` if `key` is present in `map`. Otherwise, returns -1.
 */
int32_t int32_map_remove(int32_map_t *map, int32_t key)
{
    uint32_t hash = jenkins_hash_uint32((int32_t) key) & (map->capacity - 1);
    if (map->data[hash] == NULL) {
        return -1;
    }
    
    int32_map_entry_t *cur, *prev;
    int32_t value;

    cur = map->data[hash];
    prev = NULL;
    value = -1;
    while (cur != NULL) {
        if (cur->key == key) {
            value = cur->value;
            if (prev == NULL) {
                map->data[hash] = cur->next;
            } else {
                prev->next = cur->next;
            }
            free(cur);
            return value;
        }
        prev = cur;
        cur = cur->next;
    }
    return -1;
}
