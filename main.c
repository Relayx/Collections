#include <stdio.h>

#define COLLECTIONS_CANARY_PROTECT
#define COLLECTIONS_HASH_PROTECT
#define COLLECTIONS_DUMPING

#define STACK_TYPE int
#include "lib/stack.h"
#undef STACK_TYPE

void PrintInt(int i) { printf("%d", i); }

int main() {
  Stack_int stk = {}; // Zeroing out the structure
  StackCtor(int, &stk, 5, PrintInt); // Constructs the stack
  StackPush(int, &stk, 249);
  StackPush(int, &stk, 810);
  StackPush(int, &stk, 666);

  int temp = 0;
  StackPop(int, &stk, &temp);
  printf("%d\n", temp);
  StackDump(int, &stk);

  StackDtor(int, &stk); // Deletes the stack
  return 0;
}