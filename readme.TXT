scope: Linux and ELF only with GNU toolchain. custom Libc port for startup process (this is just a demo, it only pretend to do some job on Libc side)

lifetime: libraries is automatically unloaded on process quit by OS.

shared libraries: if stub/import libraries are used in an ELF shared library, patch the LibC's start up files for shared libraries, export marker symbols for our custom sections as dynamic symbols (also be careful about visiblity, need to override as "default" explicitly) so we can get those in LibC's dynamic linker when we needed to call our callbacks.
