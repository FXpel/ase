#include <stdio.h>
#include <stdlib.h>
#include "frmt.h"
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

void format1(int cycl,int sect){
  seek(cycl,sect);
  _out(HDA_DATAREGS, 0x00);
  _out(HDA_DATAREGS + 1, 0x01);
  _out(HDA_DATAREGS + 2, 0x00);
  _out(HDA_DATAREGS + 3, 0x00);
  _out(HDA_DATAREGS + 4, 0x00);
  _out(HDA_DATAREGS + 5, 0x00);
  _out(HDA_CMDREGS,8);
  _sleep(HDA_IRQ);
}

void my_format(int nbcycl,int nbsect){
  int i,j;
  for(i=0; i<nbcycl; i++){
    for(j=0; j<nbsect; j++){
      printf("lancement commande\n");
      format1(i,j);
    }
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
    my_format(1,2);
    /* and exit! */
    exit(EXIT_SUCCESS);
}
