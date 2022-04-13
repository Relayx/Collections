#ifdef STACK_TYPE

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/template.h"
#include "core/error.h"
#include "core/info.h"
#include "core/protection.h"
#include "core/dump.h"

#ifndef RELAYX_COLLECTIONS_STACK_UTILITIES
#define RELAYX_COLLECTIONS_STACK_UTILITIES

static const uint8_t EXPAND_VALUE = 2;

// ----------------------->>> Wrappers for stack methods <<<-----------------------

#define StackCtor(TYPE, STACK, CAPACITY, PRINT) \
  TEMPLATE(_StackCtor, TYPE)(STACK, CAPACITY /*,*/ DUMP_ARG(PRINT) /*,*/ DUMP_ARG(GET_CINFO(TYPE, STACK)))

#define StackDtor(TYPE, STACK) \
  TEMPLATE(_StackDtor, TYPE)(STACK /*,*/ DUMP_ARG(GET_CINFO(TYPE, STACK)))

#define StackPush(TYPE, STACK, VALUE) \
  TEMPLATE(_StackPush, TYPE)(STACK, VALUE /*,*/ DUMP_ARG(GET_CINFO(TYPE, STACK)))

#define StackPop(TYPE, STACK, VALUE) \
  TEMPLATE(_StackPop, TYPE)(STACK, VALUE /*,*/ DUMP_ARG(GET_CINFO(TYPE, STACK)))

#define StackSize(TYPE, STACK, VALUE) \
  TEMPLATE(_StackSize, TYPE)(STACK, VALUE /*,*/ DUMP_ARG(GET_CINFO(TYPE, STACK)))

#define StackVerify(TYPE, STACK) \
    TEMPLATE(_StackVerify, TYPE)(STACK)

#define STACK_FOREACH(TYPE, VARIABLE, STACK, CODE) \
{ \
  TYPE VARIABLE; \
  for (int i = 0; i < STACK .size; ++i) { \
      VARIABLE = STACK .data[i]; \
      CODE \
  } \
}

// ---------------------------->> Dumping for stack <<<----------------------------
#ifdef COLLECTIONS_DUMPING //------------------------------------------------------

#define StackDump(TYPE, STACK) \
  TEMPLATE(_StackDump, TYPE)(STACK, \
  GET_CINFO(TYPE, STACK), \
  TEMPLATE(_StackVerify, TYPE)(STACK))

#else // --------------------------------------------------------------------------

#define StackDump(TYPE, STACK)

///////////////////////////////////////////////////////////////////////////////////
#endif // COLLECTIONS_DUMPING -----------------------------------------------------

// ------------------------->>> Verification for stack <<<-------------------------
#ifdef COLLECTIONS_DUMPING //------------------------------------------------------

#define __STACK_OK \
{ \
  CErrorSet error = StackVerify(STACK_TYPE, stack); \
    if (error != C_OK) \
      return StackDump(STACK_TYPE, stack); \
}

#else // --------------------------------------------------------------------------

#define __STACK_OK \
{ \
  CErrorSet error = StackVerify(STACK_TYPE, stack); \
    if (error != C_OK) \
      return error; \
}

///////////////////////////////////////////////////////////////////////////////////
#endif // COLLECTIONS_DUMPING -----------------------------------------------------

#define __STACK_DROP_ERROR(EXPRESSION, ERROR) \
  EXCEPTION(EXPRESSION, ERROR, \
  TEMPLATE(_StackDump, STACK_TYPE)(stack, GET_CINFO(TYPE, STACK), (1u << ERROR)))

#endif // RELAYX_COLLECTIONS_STACK_UTILITIES --------------------------------------
///////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
// -------------------------->>> Stack initialization <<<--------------------------

#define STACK TEMPLATE(Stack, STACK_TYPE)
// typedef struct _STACK_T {...} Stack_T;

typedef struct TEMPLATE(_Stack, STACK_TYPE) {
/*
          ↱ (canaryLeft)      ↱ (canaryRight)
    ---+---+---+---+---+---+---+---+---
       | L |   | D |   |   | R |   |     --->>> Canary protection for struct.
    ---+---+---+---+---+---+---+---+--- 
                 |
                 |
                 ↓
    ---+---+---+---+---+---+---+---+---+---
       |   | L |   |   |   |   |   | R |     --->>> Canary protection for data.
    ---+---+---+---+---+---+---+---+---+--- 
             ↪ (canaryLeft)          ↪ (canaryRight)
*/
#ifdef COLLECTIONS_CANARY_PROTECT
    CCanary canaryLeft;
#endif // COLLECTIONS_CANARY_PROTECT

    size_t      size;
    size_t      capacity;
    STACK_TYPE* data;

#ifdef COLLECTIONS_DUMPING
    void (*PrintElement)(STACK_TYPE);
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

} STACK;
///////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
// -------------------->>> Dumping and ErrorsChecking tools <<<--------------------

CErrorSet TEMPLATE(_StackVerify, STACK_TYPE)(STACK* stack)
{
    CErrorSet error = 0;

    if (!stack)       return (1u << C_NULL_REFERENCE);
    if (!stack->data) return (1u << C_NULL_REFERENCE_DATA);

    if (stack->size > stack->capacity) error |= (1u << C_BAD_VALUES);

#ifdef COLLECTIONS_CANARY_PROTECT

    if (stack->canaryLeft != CANARY_VALUE)  error |= (1u << C_STRUCT_LEFT_CANARY_FAULT);
    if (stack->canaryRight != CANARY_VALUE) error |= (1u << C_STRUCT_RIGHT_CANARY_FAULT);
    
    if (*(GetStartCanary((char*)stack->data)) != CANARY_VALUE)
        error |= (1u << C_DATA_LEFT_CANARY_FAULT);

    if (*(GetEndCanary((char*)stack->data, 
                       stack->capacity * sizeof(STACK_TYPE))) != CANARY_VALUE)
        error |= (1u << C_DATA_RIGHT_CANARY_FAULT);

#endif // COLLECTIONS_CANARY_PROTECT

#ifdef COLLECTIONS_HASH_PROTECT

    if (stack->hash != HashSum((char*)stack->data, stack->capacity * sizeof(STACK_TYPE)))
        error |= (1u << C_DATA_HASH_FAULT);

    if (stack->structHash != HashSum((char*)stack, sizeof(STACK) - sizeof(Chash)))
        error |= (1u << C_STRUCT_HASH_FAULT);

#endif // COLLECTIONS_HASH_PROTECT

    return error;
}

//---------------------------------------------------------------------------------

#ifdef COLLECTIONS_DUMPING

CErrorSet TEMPLATE(_StackDump, STACK_TYPE)(STACK* stack, CInfo info, CErrorSet error)
{
    // Colors for console log
    const char* const RESET  ="\033[0m";
    const char* const RED    ="\033[1;31m";
    const char* const GREEN  = "\033[1;32m";
    const char* const YELLOW = "\033[1;33m";
    const char* const BLUE   = "\033[1;34m";

    printf("Stack<%s%s%s>[%s%p%s] \"%s%s%s\" at function: %s%s%s at file: %s%s(%zu)%s\n",
           YELLOW, info.object_type, RESET, BLUE, stack, RESET, YELLOW, info.variable_name, RESET, YELLOW,
           info.function, RESET, YELLOW, info.file, info.line, RESET);

    printf("Status: ");

    if (error == C_OK)
    {
        printf("%s%s%s\n", GREEN, ERROR_INFO[C_OK] ,RESET);
    }
    else
    {
        int errorsNum = 0;

        for (unsigned int i = 1; i < C_CERROR_SIZE; ++i)
            if (error & (1u << i)) ++errorsNum;

        printf("%sErrors(%d)%s:\n", RED, errorsNum, RESET);

        for (unsigned int i = 1; i < C_CERROR_SIZE; ++i)
        {
            if (error & (1u << i))
                printf("%s%s <<- ERROR_CODE(%d)%s\n", RED, ERROR_INFO[i], i, RESET);
            if (error & (1u << C_NULL_REFERENCE)) return error;
        }
    }
    
    printf("{\n");

    printf("\t%ssize%s = %zu\n\t%scapacity%s = %zu\n\t%sdata%s[%s%p%s]\n",
        YELLOW, RESET, stack->size, YELLOW, RESET,
        stack->capacity, YELLOW, RESET, BLUE, stack->data, RESET);

    if (error & (1u << C_NULL_REFERENCE_DATA))
    {
        printf("}\n");
        return error;
    }

    printf("\t{\n");

    for (size_t i = 0; i < stack->capacity; ++i)
    {
        if (i < stack->size) printf("\t\t%s[%zu]%s = ", GREEN, i, RESET);
        else printf("\t\t%s[%zu]%s = ", RED, i, RESET);
        
        if (stack->PrintElement == NULL)
        {
            printf("%s!Print function is not assigned!%s\n", RED, RESET);
            continue;
        }
        else stack->PrintElement(stack->data[i]);

        if (i >= stack->size) printf(" %s(TRASH)%s\n", RED, RESET);
        else printf("\n");
    }

    printf("\t}\n");
    printf("}\n");

    return error;
}

#endif // COLLECTIONS_DUMPING -----------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
// ------------------------->>> Stack native functions <<<-------------------------

CErrorSet TEMPLATE(_StackCtor, STACK_TYPE)(STACK* stack, size_t capacity /*,*/
          DUMP_ARG(void (*PrintElement)(STACK_TYPE)) /*,*/ DUMP_ARG(CInfo info))
{
    __STACK_DROP_ERROR(!stack, C_NULL_REFERENCE); // !Error
    __STACK_DROP_ERROR(stack->data != NULL, C_ALREADY_CONSTRUCTED); // !Error
    __STACK_DROP_ERROR(stack->capacity != 0, C_ALREADY_CONSTRUCTED); // !Error
    __STACK_DROP_ERROR(stack->size != 0, C_ALREADY_CONSTRUCTED); // !Error

#ifdef COLLECTIONS_CANARY_PROTECT

    char* memory = (char*) calloc((capacity * sizeof(STACK_TYPE)) + 
                                  (2 * sizeof(CCanary)), sizeof(char));
    __STACK_DROP_ERROR(memory == NULL, C_OUT_OF_MEMORY); // !Error
    memory += sizeof(CCanary);

    stack->canaryLeft                                      = CANARY_VALUE;
    stack->canaryRight                                     = CANARY_VALUE;
    *(GetStartCanary(memory))                              = CANARY_VALUE;
    *(GetEndCanary(memory, capacity * sizeof(STACK_TYPE))) = CANARY_VALUE;

#else

    STACK_TYPE* memory = (STACK_TYPE*) calloc(capacity, sizeof(STACK_TYPE));
    __STACK_DROP_ERROR(memory == NULL, C_OUT_OF_MEMORY); // !Error

#endif // COLLECTIONS_CANARY_PROTECT

    stack->data     = (STACK_TYPE*) memory;
    stack->capacity = capacity;
    stack->size     = 0;

#ifdef COLLECTIONS_DUMPING
    stack->PrintElement = PrintElement;
#endif // COLLECTIONS_DUMPING

#ifdef COLLECTIONS_HASH_PROTECT
    stack->hash       = HashSum((char*)stack->data, stack->capacity * sizeof(STACK_TYPE));
    stack->structHash = HashSum((char*)stack, sizeof(STACK) - sizeof(Chash));
#endif // COLLECTIONS_HASH_PROTECT

    __STACK_OK

    return C_OK;
}

//---------------------------------------------------------------------------------

CErrorSet TEMPLATE(_StackDtor, STACK_TYPE)(STACK* stack /*,*/ DUMP_ARG(CInfo info))
{
    __STACK_DROP_ERROR(!stack, C_NULL_REFERENCE); // !Error
    __STACK_DROP_ERROR(stack->data == NULL && stack->capacity == 0 && stack->size == 0, 
               C_ALREADY_DELETED); // !Error
    __STACK_OK

#ifdef COLLECTIONS_CANARY_PROTECT

    char* memory = (char*)stack->data - sizeof(CCanary);
    memset(memory, '\0', (stack->capacity * sizeof(STACK_TYPE)) + 
                       + (2 * sizeof(CCanary)));

#else

    char* memory = (char*)stack->data;
    memset(memory, '\0', stack->capacity * sizeof(STACK_TYPE));

#endif // COLLECTIONS_CANARY_PROTECT

    free(memory);

    stack->data     = NULL;
    stack->size     = 0;
    stack->capacity = 0;

    return C_OK;
}

//---------------------------------------------------------------------------------

CErrorSet TEMPLATE(_StackResize, STACK_TYPE)(STACK* stack, size_t capacity /*,*/ DUMP_ARG(CInfo info))
{
    __STACK_OK
    
    stack->capacity = capacity;

#ifdef COLLECTIONS_CANARY_PROTECT

    char* memory = (char*)stack->data - sizeof(CCanary);
    memory = (char*) realloc(memory, (stack->capacity * sizeof(STACK_TYPE)) + 
                                   + (2 * sizeof(CCanary)));
    __STACK_DROP_ERROR(memory == NULL, C_OUT_OF_MEMORY); // !Error

    stack->data = (STACK_TYPE*)(memory + sizeof(CCanary));

    *(GetStartCanary((char*)stack->data))                                     = CANARY_VALUE;
    *(GetEndCanary((char*)stack->data, stack->capacity * sizeof(STACK_TYPE))) = CANARY_VALUE;

#else

    char* memory = (char*) realloc(stack->data, stack->capacity * sizeof(STACK_TYPE));
    __STACK_DROP_ERROR(memory == NULL, C_OUT_OF_MEMORY); // !Error
    stack->data = (STACK_TYPE*) memory;

#endif // COLLECTIONS_CANARY_PROTECT

#ifdef COLLECTIONS_HASH_PROTECT
    stack->hash       = HashSum((char*)stack->data, stack->capacity * sizeof(STACK_TYPE));
    stack->structHash = HashSum((char*)stack, sizeof(STACK) - sizeof(Chash));
#endif // COLLECTIONS_HASH_PROTECT

    __STACK_OK

    return C_OK;
}

//---------------------------------------------------------------------------------

CErrorSet TEMPLATE(_StackPush, STACK_TYPE)(STACK* stack, STACK_TYPE value /*,*/ DUMP_ARG(CInfo info))
{
    __STACK_OK

    if (stack->size >= stack->capacity)
    {
        CErrorSet error = TEMPLATE(_StackResize, STACK_TYPE)
            (stack, stack->capacity * EXPAND_VALUE /*,*/ DUMP_ARG(info));
        if (error != C_OK) return error;
    }
    
    stack->data[stack->size++] = value;

#ifdef COLLECTIONS_HASH_PROTECT
    stack->hash       = HashSum((char*)stack->data, stack->capacity * sizeof(STACK_TYPE));
    stack->structHash = HashSum((char*)stack, sizeof(STACK) - sizeof(Chash));
#endif // COLLECTIONS_HASH_PROTECT

    __STACK_OK

    return C_OK;
}

//---------------------------------------------------------------------------------

CErrorSet TEMPLATE(_StackPop, STACK_TYPE)(STACK* stack, STACK_TYPE* value /*,*/ DUMP_ARG(CInfo info))
{
    __STACK_OK
    __STACK_DROP_ERROR(!value, C_NULL_REFERENCE); // !Error
    __STACK_DROP_ERROR(stack->size == 0, C_BAD_VALUES); // !Error

    *value = stack->data[--stack->size];

#ifdef COLLECTIONS_HASH_PROTECT
    stack->hash       = HashSum((char*)stack->data, stack->capacity * sizeof(STACK_TYPE));
    stack->structHash = HashSum((char*)stack, sizeof(STACK) - sizeof(Chash));
#endif // COLLECTIONS_HASH_PROTECT

    if (stack->size < stack->capacity / (EXPAND_VALUE * 2)) 
    {
        CErrorSet error = TEMPLATE(_StackResize, STACK_TYPE)
            (stack, stack->capacity / EXPAND_VALUE /*,*/ DUMP_ARG(info));
        if (error != C_OK) return error;
    }

    __STACK_OK

    return C_OK;
}

//---------------------------------------------------------------------------------

CErrorSet TEMPLATE(_StackSize, STACK_TYPE)(STACK* stack, size_t* size /*,*/ DUMP_ARG(CInfo info))
{
    __STACK_OK
    __STACK_DROP_ERROR(!size, C_NULL_REFERENCE); // !Error

    *size = stack->size;

    return C_OK;
}

///////////////////////////////////////////////////////////////////////////////////

#undef STACK

#endif // STACK_TYPE --------------------------------------------------------------