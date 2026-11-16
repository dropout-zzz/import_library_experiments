typedef void (*resolver_callback)(void);

#define REGISTER_RESOLVER_CALLBACK __attribute__((section("dotresolve"),used,retain)) static const resolver_callback

typedef void (*dlopen_callback)(void);

#define REGISTER_DLOPEN_CALLBACK __attribute__((section("dotdlopen"),used,retain)) static const dlopen_callback
