// handy helps for our testing purpose
// the actual production will use a script to generate stub files so it may not need a shared header like this then.
// the production script would directly generate naked function with a basic inline asm statement to make function of any argument or prototype work. (the stub function do not need a calling convention or real prototype but can be a dummy one since we dont use those, there is no lazy binding too.)

typedef void (*resolver_callback)(void);

// GNU ld documents: when a undefined symbol is occurred (the user app code uses a library API), it looks for linked archive and extracts the object file that provides it. then we have this `retain` attribute to force LD to emit this variable (also a documented behavior) holding the callback function (so it pulls in callback function too).
// eventually runtime.c will ensure all initialize is done before user app code is ran. so what we have implmeneted is a immediate binding approach.
// lazy binding would be similar except we need save/restore registers. another complication that introduce is: word tearing as C doesn't assume normal variable to be modified from other threads (we may need locking or atomic stuffs, however i need to investigate the performance costs for EVERY FUNCTION INVOCATION, that is why we want to default to immediate binding for now. after all, we made sure to reduce the cost of immediate binding by only loading the library that is used and only resolve functions that is used. i suppose lazy binding is only useful to implement if benchmark tells us anything in future.).
#define REGISTER_RESOLVER_CALLBACK __attribute__((section("dotresolve"),used,retain)) static const resolver_callback

typedef void (*dlopen_callback)(void);

// each function stub has its own object file, once app uses any library function, linker pulls in the object with stub, which emits a resolver callback plus a reference to library handle. so library will be loaded once before any resolver run (see runtime.c).
// NOTE: these are going to run before global constuctors in the actual production runtime environment, this is possible as we modify the Libc port, in this demo however we are not handling anything of that (nor used any constructor) for simplicity.
#define REGISTER_DLOPEN_CALLBACK __attribute__((section("dotdlopen"),used,retain)) static const dlopen_callback

// the intention is if our "import library" (aka "stub libraries") is linked into a shared library in future, it wouldnt export the private symbols we use internally.
#define EXPORT_API __attribute__((visibility("default")))

// we need to do this since in production the stub is a naked function with basic asm stmt, compiler might see func ptr var is written but not read and that causes it to be optimized out.
// it seems like if you use a custom C identifier named section you want the `retain` but just `used` is enough for normal variables? im confused
#define FUNC_PTR_VAR __attribute__((used))
