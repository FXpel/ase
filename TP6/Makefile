# $Id: Makefile,v 1.2 2004/10/12 09:06:17 marquet Exp $
##############################################################################

ROOTDIR=/home/enseign/ASE

CC	= gcc
CFLAGS	= -m32 -Wall -ansi -fno-pic
CFLAGS  += -g -O0
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

mkvolv1.o: mkvolv1.c mbr.h
	$(CC) $(CFLAGS) -c mkvolv1.c $(INCDIR)
mkvolv1 : mkvolv1.o mbr.o drive.o frmt.o
	$(CC) $(CFLAGS) -o mkvolv1 mkvolv1.o mbr.o drive.o frmt.o $(LIBS)
frmt.o: frmt.h frmt.c
	$(CC) $(CFLAGS) -c frmt.c $(INCDIR)
frmt : frmt.o
	$(CC) $(CFLAGS) -o frmt frmt.o $(LIBS)
drive.o: drive.h drive.c
	$(CC) $(CFLAGS) -c drive.c $(INCDIR)
drive : drive.o
	$(CC) $(CFLAGS) -o drive drive.o $(LIBS)
mbr.o:  mbr.h drive.h frmt.h mbr.c
	$(CC) $(CFLAGS) -c mbr.c $(INCDIR)
mbr: drive.o frmt.o mbr.o
	$(CC) $(CFLAGS) -o mbr mbr.o frmt.o drive.o $(LIBS)
vm.o: vm.c mbr.h drive.h frmt.h
	$(CC) $(CFLAGS) -c vm.c $(INCDIR)
vm: vm.o
	$(CC) $(CFLAGS) -o vm vm.o $(LIBS)

###------------------------------
### Misc.
###------------------------------------------------------------
.PHONY: clean realclean depend
clean:
	$(RM) *.o $(BINARIES)
	$(RM) mbr
	$(RM) frmt
	$(RM) drive
	$(RM) mkvolv1
realclean: clean
	$(RM) vdiskA.bin vdiskB.bin
