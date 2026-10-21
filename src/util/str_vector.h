#ifndef __STR_VECTOR_H__
#define __STR_VECTOR_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct str_vector {
    uint32_t capacity;
    uint32_t size;
    char **data;
} str_vector_t;

#define STR_VECTOR_MAX_SIZE (UINT32_MAX / 2)
#define DEFAULT_STR_VECTOR_SIZE 1024

void init_str_vector(str_vector_t *vec, uint32_t size);
void delete_str_vector(str_vector_t *vec);
void str_vector_reset(str_vector_t *vec);
void str_vector_append(str_vector_t *vec, char *str);

#endif
