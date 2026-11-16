#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

extern void *lib1_handle;

FUNC_PTR_VAR static void (*lib1_func2_ptr)(void);

STUB void l1_f2(void) {
  __asm__("jmp *lib1_func2_ptr(%rip)");
}

static void lib1_func2_resolver_cb(void) {
  printf("lib1 func2 is being resolved.\n");
  void * sym = dlsym(lib1_handle, "l1_f2");
  if (!sym) {
    fprintf(stderr, "failed to resolve lib1 func2.\n");
    abort();
  }
  lib1_func2_ptr = (__typeof__(lib1_func2_ptr))sym;
}

REGISTER_RESOLVER_CALLBACK lib1_func2_resolver = lib1_func2_resolver_cb;
