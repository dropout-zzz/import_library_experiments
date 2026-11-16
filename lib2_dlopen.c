#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

void *lib2_handle;

static void lib2_dlopen(void) {
  printf("lib2 is being loaded\n");
  lib2_handle = dlopen("./lib2.so", RTLD_LAZY);
  if(!lib2_handle) {
    fprintf(stderr, "failed to load lib2.\n");
    abort();
  }
}

REGISTER_DLOPEN_CALLBACK lib2_dlopen_callback = lib2_dlopen;
