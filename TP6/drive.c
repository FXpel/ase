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
    unsigned int i;
    seek(cylinder,sector);
    _out(HDA_DATAREGS,NB_SECT >> 8);
    _out(HDA_DATAREGS + 1 ,NB_SECT & 0xFF);
    _out(HDA_CMDREGS,READ_CMD);
    printf("lancement commande lecture secteur\n");
    _sleep(HDA_IRQ);
    for (i = 0; i < BUFFSIZE; i++) {
      buffer[i] = MASTERBUFFER[i];
    }
  }

void write_sector(unsigned int cylinder, unsigned int sector,
  const unsigned char *buffer){
    unsigned int i;
    seek(cylinder,sector);
    _out(HDA_DATAREGS,NB_SECT >> 8);
    _out(HDA_DATAREGS + 1 ,NB_SECT & 0xFF);
    for (i = 0; i < BUFFSIZE; i++) {
      MASTERBUFFER[i] = buffer[i];
    }
    _out(HDA_CMDREGS,WRITE_CMD);
    printf("lancement commande écriture secteur\n");
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
    unsigned int i,j;
    for(i=0; i<ncylinder; i++){
      for(j=0; j<nsector; j++){
        printf("lancement commande format secteur\n");
        format1(i,j,nsector,value);
      }
    }

                   }
