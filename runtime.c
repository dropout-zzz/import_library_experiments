/*
 * "runtime.c" here emulates if we had Libc side support
 * user main is pretend to be at "app.c" as app_main with a dummy prototype for test purpose
 *
 * note that if we support dynamic linking and shared libraries, then the "import library" might be linked into a shared library.
 * in that case it will have its own copy of library handle though dynamic linker ensures there is one of the actual library instance.
 * still note it require modifying dynamic linker to run its callbacks in our custom sections. (this should be done when the library is loaded due to any reason and its the same immediate binding as we do in this file i.e. before running user "main" code)
 */

#include <stdint.h>
#include <stdio.h>

#include "common.h"

extern void app_main(void);

// no-op dummy to ensure we link when app uses nothing.
static void dummy_resolver(void) {}
REGISTER_RESOLVER_CALLBACK dummy_resolver_entry = dummy_resolver;

// we need to take the address of these symbols, not their values. (do they even have meaningful values? we dont care that anyways.)
// we are defining __start_* as an external array so it satisfies the above note.
extern const resolver_callback __start_dotresolve[];
// we dont access any data via it so it can just a `const void` and we only take reference of it to get its location later.
extern const void __stop_dotresolve;

static void dummy_dlopen_cb(void) {}
REGISTER_DLOPEN_CALLBACK dummy_dlopen_entry = dummy_dlopen_cb;

extern const dlopen_callback __start_dotdlopen[];
extern const void __stop_dotdlopen;

int main(void) {
  int dlopen_count = ((uintptr_t)&__stop_dotdlopen - (uintptr_t)__start_dotdlopen) / sizeof(__start_dotdlopen[0]);

  printf("applying %d dlopen()s\n", dlopen_count - 1);

  for (int i = 0; i < dlopen_count; i++)
    __start_dotdlopen[i]();

  int resolve_count = ((uintptr_t)&__stop_dotresolve - (uintptr_t)__start_dotresolve) / sizeof(__start_dotresolve[0]);

  printf("applying %d dlsym()s\n", resolve_count - 1);

  for (int i = 0; i < resolve_count; i++)
    __start_dotresolve[i]();

  app_main();

  printf("app main is returned\n");
}
