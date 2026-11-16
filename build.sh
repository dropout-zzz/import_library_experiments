set -e

rel="-O2 -flto -Wall -Wextra -Werror -std=gnu99 -ffunction-sections -fdata-sections"
llclang="clang $rel"
cc="gcc $rel"
ldfl="-Wl,--gc-sections"

$cc lib1_impl.c -o lib1.so -shared
$cc lib2_impl.c -o lib2.so -shared

$cc -c lib1_dlopen.c
$cc -c lib1_func1_stub.c
$cc -c lib1_func2_stub.c
$cc -c lib2_dlopen.c
$cc -c lib2_func1_stub.c
$cc -c lib2_func2_stub.c
ar rcs liblib1.a lib1_func1_stub.o lib1_func2_stub.o lib1_dlopen.o
ar rcs liblib2.a lib2_func1_stub.o lib2_func2_stub.o lib2_dlopen.o

$llclang -c lib1_dlopen.c -o l1dl.o
$llclang -c lib1_func1_stub.c -o l1f1_ll.o
$llclang -c lib1_func2_stub.c -o l1f2_ll.o
$llclang -c lib2_dlopen.c -o l2dl.o
$llclang -c lib2_func1_stub.c -o l2f1_ll.o
$llclang -c lib2_func2_stub.c -o l2f2_ll.o
ar rcs liblib1_llvm.a l1f1_ll.o l1f2_ll.o l1dl.o
ar rcs liblib2_llvm.a l2f1_ll.o l2f2_ll.o l2dl.o

$cc runtime.c app.c -o app -L. -llib1 -llib2 $ldfl
$llclang -fuse-ld=lld runtime.c app.c -o app_llvm -L. -llib1_llvm -llib2_llvm $ldfl
