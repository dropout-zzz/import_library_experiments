#!/bin/bash

# quick and dirty build script, rebuilds everything every time it runs
# this test both GCC and LLVM/Clang toolchain

set -e

rel="-g -O2 -flto -Wall -Wextra -Werror -std=gnu99 -ffunction-sections -fdata-sections -fvisibility=hidden"
llclang="clang $rel"
cc="gcc $rel"
ldfl="-Wl,--gc-sections"

# not building again with Clang bcs not important.
$cc lib1_impl.c -o lib1.so -shared -fPIC $ldfl
$cc lib2_impl.c -o lib2.so -shared -fPIC $ldfl

$cc -c lib1_dlopen.c -fPIC
$cc -c lib1_func1_stub.c -fPIC
$cc -c lib1_func2_stub.c -fPIC
$cc -c lib2_dlopen.c -fPIC
$cc -c lib2_func1_stub.c -fPIC
$cc -c lib2_func2_stub.c -fPIC
rm -f liblib1.a liblib2.a
# im putting dlopen at last just in case.
# func stubs needs a symbol (library handle) from the dlopen obj
ar rcs liblib1.a lib1_func1_stub.o lib1_func2_stub.o lib1_dlopen.o
ar rcs liblib2.a lib2_func1_stub.o lib2_func2_stub.o lib2_dlopen.o

$llclang -c lib1_dlopen.c -o l1dl.o -fPIC
$llclang -c lib1_func1_stub.c -o l1f1_ll.o -fPIC
$llclang -c lib1_func2_stub.c -o l1f2_ll.o -fPIC
$llclang -c lib2_dlopen.c -o l2dl.o -fPIC
$llclang -c lib2_func1_stub.c -o l2f1_ll.o -fPIC
$llclang -c lib2_func2_stub.c -o l2f2_ll.o -fPIC
rm -f liblib1_llvm.a liblib2_llvm.a
ar rcs liblib1_llvm.a l1f1_ll.o l1f2_ll.o l1dl.o
ar rcs liblib2_llvm.a l2f1_ll.o l2f2_ll.o l2dl.o

# put -l after app code!
$cc -fPIE runtime.c app.c -o app -L. -llib1 -llib2 $ldfl -pie
$llclang -fPIE -fuse-ld=lld runtime.c app.c -o app_llvm -L. -llib1_llvm -llib2_llvm $ldfl -pie
