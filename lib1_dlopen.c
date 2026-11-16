#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

// this can't be static because stub needs it to dlsym.
// it cant be const because its inited at runtime.
void *lib1_handle;

static void lib1_dlopen(void) {
  printf("lib1 is being loaded\n");
  lib1_handle = dlopen("./lib1.so", RTLD_LAZY);
  if(!lib1_handle) {
    fprintf(stderr, "failed to load lib1.\n");
    abort();
  }
}

REGISTER_DLOPEN_CALLBACK lib1_dlopen_callback = lib1_dlopen;
