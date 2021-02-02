### Problèmes :

Soucis de compilation

gcc -Wall -Wextra -ansi -pedantic -D_XOPEN_SOURCE=500 -g -c init_ctx.c
gcc -fno-pic -m32 -o switch_to switch_to.c
/tmp/ccL4MzXZ.o: In function `main':
switch_to.c:(.text+0x132): undefined reference to `init_ctx'
switch_to.c:(.text+0x14b): undefined reference to `init_ctx'
collect2: error: ld returned 1 exit status
Makefile:14: recipe for target 'switch_to.o' failed
make: *** [switch_to.o] Error 1
