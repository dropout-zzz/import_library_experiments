#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

extern void *lib1_handle;

// cant be const as inited at runtime.
static void (*lib1_func1_ptr)(void);

void l1_f1(void) {
  lib1_func1_ptr();
}

static void lib1_func1_resolver_cb(void) {
  printf("lib1 func1 is being resolved.\n");
  void * sym = dlsym(lib1_handle, "l1_f1");
  if (!sym) {
    fprintf(stderr, "failed to resolve lib1 func1.\n");
    abort();
  }
  lib1_func1_ptr = (__typeof__(lib1_func1_ptr))sym;
}

REGISTER_RESOLVER_CALLBACK lib1_func1_resolver = lib1_func1_resolver_cb;
