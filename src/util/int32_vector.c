/**
 * 
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "int32_vector.h"   

void init_int32_vector(int32_vector_t *vec, uint32_t size)
{
    if (size == 0) {
        vec->capacity = DEFAULT_INT32_VECTOR_SIZE;
    } else {
        vec->capacity = size;
    }
    vec->size = 0;
    vec->data = malloc(vec->capacity * sizeof(int32_t)); 
}

void delete_int32_vector(int32_vector_t *vec)
{
    free(vec->data);
}

void int32_vector_reset(int32_vector_t *vec)
{
    vec->size = 0;
}

void int32_vector_extend(int32_vector_t *vec, uint32_t size)
{
    size_t new_size = vec->capacity + size;
    vec->capacity = new_size;
    vec->data = realloc(vec->data, sizeof(int32_t) * new_size);
}

void int32_vector_append(int32_vector_t *vec, int32_t i)
{
    vec->data[vec->size] = i;
    vec->size++;
    if (vec->size == vec->capacity) {
        int32_vector_extend(vec, vec->capacity);
    }
}

