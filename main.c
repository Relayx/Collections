#include <stdio.h>

#define ARRAY_TYPE int
#include "lib/array.h"
#undef ARRAY_TYPE

int main() {
  Array_int array;
  ArrayCtor_int(&array, 1);
  ArrayPush_int(&array, 20);
  ArrayPush_int(&array, 30);
  ArrayPush_int(&array, 40);
  int a;
  ArrayGetAt_int(&array, 2, &a);
  printf("%d\n", a);
  return 0;
}