compil: try	  try_mul 	display_stack

try:
	gcc -fno-pic -m32 -c try.c

try_mul:
	gcc -fno-pic -m32 -o try_mul try_mul.c

display_stack:
	gcc -o display_stack display_stack.c

clean:
	rm -rf try_mul
	rm -rf display_stack
	rm -rf *.o
