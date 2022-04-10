#include <stdio.h>

#define ARRAY_TYPE int
#include "lib/array.h"
#undef ARRAY_TYPE

int main() {
  Array_int array;
  ArrayCtor_int(&array, 10);
  ArrayPush_int(&array, 20);
  int a = ArrayGetAt_int(&array, 0);
  printf("%d\n", a);
  return 0;
}