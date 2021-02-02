#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "drive.h"
#include "hwconfig.h"
#include "hardware.h"

void seek(unsigned short cyl, unsigned short sect){
  _out(HDA_DATAREGS,cyl >> 8);
  _out(HDA_DATAREGS + 1,cyl & 0xff);
  _out(HDA_DATAREGS + 2,sect >> 8);
  _out(HDA_DATAREGS + 3,sect & 0xff);
  _out(HDA_CMDREGS,SEEK_CMD);
  _sleep(HDA_IRQ);
}
void read_sector(unsigned int cylinder, unsigned int sector,
  unsigned char *buffer){
    int i;
    seek(cylinder,sector);
    _out(HDA_DATAREGS,NB_SECT >> 8);
    _out(HDA_DATAREGS + 1 ,NB_SECT & 0xFF);
    _out(HDA_CMDREGS,READ_CMD);
    printf("lancement commande \n");
    _sleep(HDA_IRQ);
    for (i = 0; i < BUFFSIZE; i++) {
      buffer[i] = MASTERBUFFER[i];
    }
  }

void write_sector(unsigned int cylinder, unsigned int sector,
  const unsigned char *buffer){
    int i;
    seek(cylinder,sector);
    _out(HDA_DATAREGS,NB_SECT >> 8);
    _out(HDA_DATAREGS + 1 ,NB_SECT & 0xFF);
    for (i = 0; i < BUFFSIZE; i++) {
      MASTERBUFFER[i] = buffer[i];
    }
    _out(HDA_CMDREGS,WRITE_CMD);
    printf("lancement commande \n");
    _sleep(HDA_IRQ);

  }

void format1(unsigned int cylinder, unsigned int sector,unsigned int nsector ,
  unsigned int value){
    seek(cylinder,sector);
    _out(HDA_DATAREGS, nsector >> 8);
    _out(HDA_DATAREGS + 1, nsector & 0xFF);
    _out(HDA_DATAREGS + 2, value >> 24);
    _out(HDA_DATAREGS + 3, (value >> 16) & 0xFF);
    _out(HDA_DATAREGS + 4, (value >> 8) & 0xFF);
    _out(HDA_DATAREGS + 5, value & 0xFF);
    _out(HDA_CMDREGS,8);
    _sleep(HDA_IRQ);
}

void format_sector(unsigned int ncylinder, unsigned int nsector,
  unsigned int value){
    int i,j;
    for(i=0; i<ncylinder; i++){
      for(j=0; j<nsector; j++){
        printf("lancement commande\n");
        format1(i,j,nsector,value);
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
   char buff[BUFFSIZE];
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
   printf("write ! \n");
   /*write_sector(1,2,buff);*/
   printf("read ! \n");
   read_sector(1,2,buff);
   printf("format ! \n");
   format_sector(1,2,0x00);
   /* and exit! */
   exit(EXIT_SUCCESS);
}
