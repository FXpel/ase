#include <stdio.h>
#include <stdlib.h>
#include "frmt.h"
#include "hwconfig.h"
#include "hardware.h"

void seek_frame(unsigned short cycl, unsigned short sect){
  _out(HDA_DATAREGS,cycl >> 8);
  _out(HDA_DATAREGS + 1,cycl & 0xff);
  _out(HDA_DATAREGS + 2,sect >> 8);
  _out(HDA_DATAREGS + 3,sect & 0xff);
  _out(HDA_CMDREGS,2);
  _sleep(HDA_IRQ);
}

void format1_fmt(int cycl,int sect){
  seek_frame(cycl,sect);
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
      format1_fmt(i,j);
    }
  }
}
