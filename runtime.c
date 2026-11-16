#include <stdint.h>
#include <stdio.h>

#include "common.h"

extern void app_main(void);

// no-op dummy to ensure we link when app uses nothing.
static void dummy_resolver(void) {}
REGISTER_RESOLVER_CALLBACK dummy_resolver_entry = dummy_resolver;

extern const resolver_callback __start_dotresolve[];
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
