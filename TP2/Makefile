CC      = gcc

CFLAGS  = -Wall -Wextra
CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -g -m32 -fno-pic

LDFLAGS = -g -m32

# Pour construire le programme binaire « args », il est nécessaire de
# faire l’édition des liens de tous les .o qui contiennent son code
# « args » dépend donc de « args.o » mais aussi de « makeargv.o »
init_ctx.o:init_ctx.c init_ctx.h
switch_to.o:switch_to.c init_ctx.h switch_to.h
switch_to:switch_to.o init_ctx.o

# Règles implicites pour la compilation et l’édition de liens
%.o: %.c
	${CC} ${CFLAGS} -c $<

%: %.o
	${CC} ${LDFLAGS} -o $@ $^

%: %.c

# Règles de nettoyage
clean:
	rm -rf switch_to
	rm -rf *.o
