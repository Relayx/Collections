#ifdef ARRAY_TYPE

#include <stdlib.h>

#include "core/template.h"

#define ARRAY TEMPLATE(Array, ARRAY_TYPE)
typedef struct {
  size_t      size;
  size_t      capacity;
  ARRAY_TYPE* data;
} ARRAY;

void TEMPLATE(ArrayCtor, ARRAY_TYPE)(ARRAY* array, size_t capacity) {
  array->data = (ARRAY_TYPE*) calloc(capacity, sizeof(ARRAY_TYPE));
  array->size = 0;
  array->capacity = capacity;
}

void TEMPLATE(ArrayDtor, ARRAY_TYPE)(ARRAY* array) {
  free(array->data);
}

void TEMPLATE(ArrayResize, ARRAY_TYPE)(ARRAY* array, size_t capacity) {
  array->capacity = capacity;
  array->data = (ARRAY_TYPE*) realloc(array->data, array->capacity * sizeof(ARRAY_TYPE));
}

void TEMPLATE(ArrayPush, ARRAY_TYPE)(ARRAY* array, int value) {
  array->data[array->size++] = value;
}

int TEMPLATE(ArrayGetAt, ARRAY_TYPE)(ARRAY* array, size_t index) {
  return array->data[index];
}

#undef ARRAY

#endif // ARRAY_TYPE
