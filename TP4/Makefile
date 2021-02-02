ROOTDIR=/home/enseign/ASE

CC      = gcc
CFLAGS  = -Wall -Wextra
CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -g -m32

INCS = -I$(ROOTDIR)/include
LIBS = -L$(ROOTDIR)/lib -lhardware

# Règles implicites pour la compilation et l’édition de liens
init_ctx.o : init_ctx.c init_ctx.h
	${CC} ${CFLAGS} -c $<

sem.o : sem.c sem.h init_ctx.h
	${CC} ${CFLAGS} $(INCS) -c $<
start_schedule.o : init_ctx.h start_schedule.c hwconfig.h
	$(CC) $(CFLAGS) $(INCS) -c start_schedule.c

start_schedule : start_schedule.o init_ctx.o
	$(CC) $(CFLAGS) -o start_schedule start_schedule.o init_ctx.o $(LIBS)

sem : sem.o init_ctx.o start_schedule.o
	$(CC) $(CFLAGS) -o sem sem.o init_ctx.o start_schedule.o $(LIBS)

main.o: main.c sem.h init_ctx.h
	$(CC) $(CFLAGS) $(INCS) -c start_schedule.c
# Règles de nettoyage
clean:
	rm -rf *.o
