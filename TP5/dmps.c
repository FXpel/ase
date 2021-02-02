#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "dmps.h"
#include "hwconfig.h"
#include "hardware.h"


void seek(unsigned short cycl, unsigned short sect){
  _out(HDA_DATAREGS,cycl >> 8);
  _out(HDA_DATAREGS + 1,cycl & 0xff);
  _out(HDA_DATAREGS + 2,sect >> 8);
  _out(HDA_DATAREGS + 3,sect & 0xff);
  _out(HDA_CMDREGS,2);
  _sleep(HDA_IRQ);
}

void my_display(short int cycl,short int sect){
  int i;
  seek(cycl,sect);
  _out(HDA_DATAREGS,0);
  _out(HDA_DATAREGS + 1 ,1);
  _out(HDA_CMDREGS,4);
  printf("lancement commande \n");
  _sleep(HDA_IRQ);
  for (i = 0; i < 1024; i++) {
    putchar(MASTERBUFFER[i]);
  }
}

static void
empty_it()
{
    return;
}

int
main(int argc, char **argv)
{
    unsigned int i;

    /* init hardware */
    if(init_hardware("hwconfig.ini") == 0) {
	fprintf(stderr, "Error in hardware initialization\n");
	exit(EXIT_FAILURE);
    }

    /* Interreupt handlers */
    for(i=0; i<16; i++)
	IRQVECTOR[i] = empty_it;

    /* Allows all IT */
    _mask(1);
    my_display(1,2);
    /* and exit! */
    exit(EXIT_SUCCESS);
}
