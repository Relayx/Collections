#ifdef ARRAY_TYPE

#include <stdlib.h>
#include <string.h>

#include "core/template.h"
#include "core/error.h"
#include "core/info.h"
#include "core/protection.h"
#include "core/dump.h"

///////////////////////////////////////////////////////////////////////////////////
// -------------------------->>> Array initialization <<<--------------------------

#define ARRAY TEMPLATE(Array, ARRAY_TYPE)

typedef struct TEMPLATE(_ARRAY, ARRAY_TYPE) {

#ifdef COLLECTIONS_CANARY_PROTECT
  CCanary canaryLeft;
#endif // COLLECTIONS_CANARY_PROTECT

  size_t      size;
  size_t      capacity;
  ARRAY_TYPE* data;

#ifdef COLLECTIONS_DUMPING
  void (*PrintElement)(ARRAY_TYPE);
#endif // COLLECTIONS_DUMPING

#ifdef COLLECTIONS_HASH_PROTECT
  Chash hash;
#endif // COLLECTIONS_HASH_PROTECT

#ifdef COLLECTIONS_CANARY_PROTECT
  CCanary canaryRight;
#endif // COLLECTIONS_CANARY_PROTECT

#ifdef COLLECTIONS_HASH_PROTECT
  Chash structHash;
#endif //COLLECTIONS_HASH_PROTECT

} ARRAY;
///////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////
// ------------------------->>> Array native functions <<<-------------------------

CError TEMPLATE(_ArrayCtor, ARRAY_TYPE)(ARRAY* array, size_t capacity /*,*/
       DUMP_ARG(void (*PrintElement)(ARRAY_TYPE)) /*,*/ DUMP_ARG(CInfo info)
) {

#ifdef COLLECTIONS_CANARY_PROTECT

  char* memory = (char*) calloc((capacity * sizeof(ARRAY_TYPE)) + 
                                (2 * sizeof(CCanary)), sizeof(char));
  // ERROR HANDLING
  memory += sizeof(CCanary);

  array->canaryLeft                                      = CANARY_VALUE;
  array->canaryRight                                     = CANARY_VALUE;
  *(GetStartCanary(memory))                              = CANARY_VALUE;
  *(GetEndCanary(memory, capacity * sizeof(ARRAY_TYPE))) = CANARY_VALUE;

#else

  ARRAY_TYPE* memory = (ARRAY_TYPE*) calloc(capacity, sizeof(ARRAY_TYPE));
  // ERROR HANDLING

#endif // COLLECTIONS_CANARY_PROTECT

  array->data     = (ARRAY_TYPE*) memory;
  array->capacity = capacity;
  array->size     = capacity;

  return C_OK;
}

//---------------------------------------------------------------------------------

CError TEMPLATE(_ArrayDtor, ARRAY_TYPE)(ARRAY* array /*,*/ DUMP_ARG(CInfo info)) {

#ifdef COLLECTIONS_CANARY_PROTECT

  char* memory = (char*)array->data - sizeof(CCanary);
  memset(memory, 0, (array->capacity * sizeof(ARRAY_TYPE)) + 
                  + (2 * sizeof(CCanary)));

#else

  char* memory = (char*)array->data;
  memset(memory, 0, array->capacity * sizeof(ARRAY_TYPE));

#endif // COLLECTIONS_CANARY_PROTECT

  free(memory);

  array->data     = NULL;
  array->size     = 0;
  array->capacity = 0;

  return C_OK;
}

//---------------------------------------------------------------------------------

CError TEMPLATE(_ArrayResize, ARRAY_TYPE)(ARRAY* array, size_t capacity /*,*/ DUMP_ARG(CInfo info)) {
  // array->capacity = capacity;

  // char* memory = (char*) realloc(array->data, array->capacity * sizeof(ARRAY_TYPE));
  // if (memory == NULL) {
  //   return C_OUT_OF_MEMORY;
  // }

  // array->data = (ARRAY_TYPE*) memory;

  // TO DO

  return C_OK;
}

//---------------------------------------------------------------------------------

CError TEMPLATE(_ArrayPush, ARRAY_TYPE)(ARRAY* array, int value /*,*/ DUMP_ARG(CInfo info)) {
  // if (array->size >= array->capacity) {
  //   CError error = TEMPLATE(ArrayResize, ARRAY_TYPE)(array, array->capacity * 2);
  //   if (error != C_OK) return error;
  // }

  // array->data[array->size++] = value;

  // TO DO

  return C_OK;
}

//---------------------------------------------------------------------------------

CError TEMPLATE(_ArrayGetAt, ARRAY_TYPE)(ARRAY* array, size_t index, 
                                         ARRAY_TYPE* out /*,*/ DUMP_ARG(CInfo info)
) {

// ERROR HANDLING (out of range)

  *out = array->data[index];

  return C_OK;
}

///////////////////////////////////////////////////////////////////////////////////

#undef ARRAY

#endif // ARRAY_TYPE --------------------------------------------------------------