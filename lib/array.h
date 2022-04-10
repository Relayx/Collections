#ifdef ARRAY_TYPE

#include <stdlib.h>
#include <string.h>

#include "core/template.h"
#include "core/error.h"

#define ARRAY TEMPLATE(Array, ARRAY_TYPE)
typedef struct {
  size_t      size;
  size_t      capacity;
  ARRAY_TYPE* data;
} ARRAY;

CError TEMPLATE(ArrayCtor, ARRAY_TYPE)(ARRAY* array, size_t capacity) {
  ARRAY_TYPE* memory = (ARRAY_TYPE*) calloc(capacity, sizeof(ARRAY_TYPE));
  if (memory == NULL) {
    return C_OUT_OF_MEMORY;
  }

  array->data     = (ARRAY_TYPE*) memory;
  array->capacity = capacity;
  array->size     = 0;

  return C_OK;
}

CError TEMPLATE(ArrayDtor, ARRAY_TYPE)(ARRAY* array) {
    char* memory = (char*) array->data;
    memset(memory, 0, array->capacity * sizeof(ARRAY_TYPE));

    free(memory);

    array->data     = NULL;
    array->size     = 0;
    array->capacity = 0;

  return C_OK;
}

CError TEMPLATE(ArrayResize, ARRAY_TYPE)(ARRAY* array, size_t capacity) {
  array->capacity = capacity;

  char* memory = (char*) realloc(array->data, array->capacity * sizeof(ARRAY_TYPE));
  if (memory == NULL) {
    return C_OUT_OF_MEMORY;
  }

  array->data = (ARRAY_TYPE*) memory;

  return C_OK;
}

CError TEMPLATE(ArrayPush, ARRAY_TYPE)(ARRAY* array, int value) {
  if (array->size >= array->capacity) {
    CError error = TEMPLATE(ArrayResize, ARRAY_TYPE)(array, array->capacity * 2);
    if (error != C_OK) return error;
  }

  array->data[array->size++] = value;

  return C_OK;
}

CError TEMPLATE(ArrayGetAt, ARRAY_TYPE)(ARRAY* array, size_t index, ARRAY_TYPE* out) {
  if (index >= array->capacity) {
    return C_OUT_OF_RANGE;
  }

  *out = array->data[index];

  return C_OK;
}

#undef ARRAY

#endif // ARRAY_TYPE
