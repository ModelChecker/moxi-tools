#ifndef __INT32_VECTOR_H__
#define __INT32_VECTOR_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct int32_vector {
    uint32_t capacity;
    uint32_t size;
    int32_t *data;
} int32_vector_t;

#define INT32_VECTOR_MAX_SIZE 65536
#define DEFAULT_INT32_VECTOR_SIZE 1024

void init_int32_vector(int32_vector_t *vec, uint32_t size);
void delete_int32_vector(int32_vector_t *vec);
void int32_vector_reset(int32_vector_t *vec);
void int32_vector_append(int32_vector_t *vec, int32_t i);

#endif
