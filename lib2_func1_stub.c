#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

extern void *lib2_handle;

FUNC_PTR_VAR static void (*lib2_func1_ptr)(void);

void l2_f1(void) {
  lib2_func1_ptr();
}

static void lib2_func1_resolver_cb(void) {
  printf("lib2 func1 is being resolved.\n");
  void * sym = dlsym(lib2_handle, "l2_f1");
  if (!sym) {
    fprintf(stderr, "failed to resolve lib2 func1.\n");
    abort();
  }
  lib2_func1_ptr = (__typeof__(lib2_func1_ptr))sym;
}

REGISTER_RESOLVER_CALLBACK lib2_func1_resolver = lib2_func1_resolver_cb;
