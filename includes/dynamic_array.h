#ifndef DARRAY_H
#define DARRAY_H
#include <stdlib.h>
#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;

#define DEFINE_ARRAY(T) \
typedef struct { \
   T *items; \
   int count; \
   int size; \
} T##_array_t; \
\
static inline T##_array_t T##_create_array(size_t size) { \
    T##_array_t array; \
    array.items = malloc(sizeof(T) * size); \
    array.count = 0; \
    array.size = size; \
    return array; \
} \
\
static inline void T##_delete_array(T##_array_t *array) { \
    free(array->items); \
    array->items = NULL; \
    array->count = 0; \
    array->size = 0; \
} \
\
static inline void T##_push_item(T##_array_t *array, T item) { \
    array->items[array->count] = item; \
    array->count++; \
} \
\
static inline void T##_append(T##_array_t *array, T item) { \
    if ((array->count + 1) < array->size) { \
        T##_push_item(array, item); \
    } else { \
        T *tmp = realloc(array->items, sizeof(T) * array->size * 2); \
        if (tmp != NULL) { \
            array->items = tmp; \
            array->size *= 2; \
            T##_push_item(array, item); \
        } \
    } \
}

#endif 
