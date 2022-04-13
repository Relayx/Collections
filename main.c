#include <stdio.h>

#define COLLECTIONS_CANARY_PROTECT
#define COLLECTIONS_HASH_PROTECT
// #define COLLECTIONS_DUMPING

#define STACK_TYPE int
#include "lib/stack.h"
#undef STACK_TYPE

#define STACK_TYPE double
#include "lib/stack.h"
#undef STACK_TYPE

#define ARRAY_TYPE int
#include "lib/array.h"
#undef STACK_TYPE

void PrintInt(int i) { printf("%d", i); }
void PrintDouble(double d) { printf("%lg", d); }

int main() {
  Stack_int stk = {}; // Zeroing out the structure
  StackCtor(int, &stk, 5, PrintInt); // Constructs the stack
  StackPush(int, &stk, 249);
  StackPush(int, &stk, 810);
  StackPush(int, &stk, 666);

  STACK_FOREACH(int, temp, stk, {
    temp -= 1000;
    printf("%d\n", temp);
  })

  StackDtor(int, &stk); // Deletes the stack

  Stack_double stack = {0};
  StackCtor(double, &stack, 5, PrintDouble);
  StackPush(double, &stack, 53.57);
  StackDump(double, &stack);
  return 0;
}