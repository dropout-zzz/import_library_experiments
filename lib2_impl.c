#include <stdio.h>

#include "common.h"

EXPORT_API void l2_f1(void) {
  printf("library 2's function 1 is called\n");
}

EXPORT_API void l2_f2(void) {
  printf("library 2's function 2 is called\n");
}
