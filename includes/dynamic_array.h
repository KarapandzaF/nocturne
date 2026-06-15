#ifndef DARRAY_H
#define DARRAY_H

#include <stdint.h>
#include <stdlib.h>
#include "track.h"

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t ui8;
typedef uint16_t ui16;
typedef uint32_t ui32;
typedef uint64_t ui64;

typedef float f32;
typedef double f64;

typedef struct track_array {
   Track *items;
   i32 count;
   i32 size;
} array_t;

array_t create_array(size_t size);
void delete_array(array_t *array);
static void push_item(array_t *array, Track item);
void append(array_t *array, Track item);

#endif 
