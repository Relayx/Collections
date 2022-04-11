#ifndef RELAYX_COLLECTIONS_ERROR
#define RELAYX_COLLECTIONS_ERROR

#include <stdint.h>

#ifdef COLLECTIONS_DUMPING //------------------------------------------------------

#define EXCEPTION(EXPRESSION, ERROR, DUMPER) \
 if (EXPRESSION) \
    return DUMPER;

#else // --------------------------------------------------------------------------

#define EXCEPTION(EXPRESSION, ERROR, DUMPER) \
 if (EXPRESSION) \
    return (1u << ERROR);

#endif // COLLECTIONS_DUMPING -----------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////

typedef uint32_t CErrorSet;

typedef enum {
  C_OK = 0,
  C_NULL_REFERENCE,
  C_NULL_REFERENCE_DATA,       
  C_OUT_OF_MEMORY,
  C_OUT_OF_RANGE, 
  C_BAD_VALUES,
  C_ALREADY_CONSTRUCTED,
  C_ALREADY_DELETED,
  C_STRUCT_LEFT_CANARY_FAULT,
  C_STRUCT_RIGHT_CANARY_FAULT,
  C_DATA_LEFT_CANARY_FAULT,
  C_DATA_RIGHT_CANARY_FAULT,
  C_STRUCT_HASH_FAULT,
  C_DATA_HASH_FAULT,
  C_CERROR_SIZE
} CError;

const char* ERROR_INFO[] = {
  "OK",                                      /* 0 */
  "Null pointer reference of stack",         /* 1 */
  "Null pointer reference of data of stack", /* 2 */
  "Out of memory",                           /* 3 */
  "Stack values was spoiled",                /* 4 */
  "Left canary of struct was damaged",       /* 5 */
  "Right canary of struct was damaged",      /* 6 */
  "Left canary of data was damaged",         /* 7 */
  "Right canary of data was damaged",        /* 8 */
  "HashSum of struct doesn't match",         /* 9 */
  "HashSum of data doesn't match",           /* 10 */
  "Stack has already constructed",           /* 11 */
  "Stack has already deleted"                /* 12 */
};

#endif // RELAYX_COLLECTIONS_ERROR
