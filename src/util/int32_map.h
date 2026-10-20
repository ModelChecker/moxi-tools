/**
 *
 */
#ifndef __INT32_MAP_H__
#define __INT32_MAP_H__

#include <stdint.h>
#include <stdlib.h>

typedef struct int32_map_entry int32_map_entry_t;

/**
 * Each entry in the map stores the int key and an int value.
 */
struct int32_map_entry {
    int32_t key;
    int32_t value;
    int32_map_entry_t *next;
};

/**
 * An integer map is a hash map of integer map entries.
 */
typedef struct int32_map {
    uint32_t capacity;
    int32_map_entry_t **data;
} int32_map_t;

#define DEFAULT_INT32_MAP_SIZE 1024

void init_int32_map(int32_map_t *map, uint32_t size);
void delete_int32_map(int32_map_t *map);
void int32_map_reset(int32_map_t *map);
int32_t int32_map_find(int32_map_t *map, int32_t key);
void int32_map_add(int32_map_t *map, int32_t key, int32_t value);
int32_t int32_map_remove(int32_map_t *map, int32_t key);

#endif
