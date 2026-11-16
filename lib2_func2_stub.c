#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

extern void *lib2_handle;

FUNC_PTR_VAR static void (*lib2_func2_ptr)(void);

STUB void l2_f2(void) {
  __asm__("jmp *lib2_func2_ptr(%rip)");
}

static void lib2_func2_resolver_cb(void) {
  printf("lib2 func2 is being resolved.\n");
  void * sym = dlsym(lib2_handle, "l2_f2");
  if (!sym) {
    fprintf(stderr, "failed to resolve lib2 func2.\n");
    abort();
  }
  lib2_func2_ptr = (__typeof__(lib2_func2_ptr))sym;
}

REGISTER_RESOLVER_CALLBACK lib2_func2_resolver = lib2_func2_resolver_cb;
