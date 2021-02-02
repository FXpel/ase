# $Id: Makefile,v 1.2 2004/10/12 09:06:17 marquet Exp $
##############################################################################

ROOTDIR=/home/enseign/ASE

CC	= gcc
CFLAGS	= -m32 -Wall -ansi -pedantic
CFLAGS  += -g
LIBDIR  = $(ROOTDIR)/lib
INCDIR  = -I$(ROOTDIR)/include
LIBS    = -L$(LIBDIR) -lhardware

###------------------------------
### Main targets
###------------------------------------------------------------
BINARIES= mkhd
OBJECTS	= $(addsuffix .o,\
	  mkhd)

all: $(BINARIES) $(OBJECTS)


###------------------------------
### Main rules
###------------------------------------------------------------
mkhd.o: mkhd.c
	$(CC) $(CFLAGS) -c mkhd.c $(INCDIR)

mkhd: mkhd.o
	$(CC) $(CFLAGS) -o mkhd mkhd.o $(LIBS)

dmps.o: dmps.h dmps.c
	$(CC) $(CFLAGS) -c dmps.c $(INCDIR)
dmps : dmps.o
	$(CC) $(CFLAGS) -o dmps dmps.o $(LIBS)

frmt.o: frmt.h frmt.c
	$(CC) $(CFLAGS) -c frmt.c $(INCDIR)
frmt : frmt.o
	$(CC) $(CFLAGS) -o frmt frmt.o $(LIBS)
drive.o: drive.h drive.c
	$(CC) $(CFLAGS) -c drive.c $(INCDIR)
drive : drive.o
	$(CC) $(CFLAGS) -o drive drive.o $(LIBS)

###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean realclean depend
clean:
	$(RM) *.o $(BINARIES)
	$(RM) dmps
	$(RM) frmt
	$(RM) drive
realclean: clean
	$(RM) vdiskA.bin vdiskB.bin
