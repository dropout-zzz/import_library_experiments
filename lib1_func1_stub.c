#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

extern void *lib1_handle;

// cant be const as inited at runtime.
static void (*lib1_func1_ptr)(void);

// in production this will be simply a naked function with a basic asm statement that simply jump immediately to func ptr which preserved any ABI or argument. the C prototype can be a dummy one. (each architecture need its own assembly port, ofc. why im unnecessarily nagging? maybe chatgpt triggered me.)
// in demo, we are not actually doing that for simplicity.
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
