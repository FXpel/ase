#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "mbr.h"
#include "frmt.h"
#include "drive.h"
#include "hwconfig.h"
#include "hardware.h"


static struct mbr_ctx global_MBR;
static int current_vol;
void load_mbr(){
    char buffer [HDA_SECTORSIZE];

    read_sector(0,0,buffer);
    memcpy(&global_MBR,buffer,sizeof(struct mbr_ctx));

    if(global_MBR.magic != MAGIC){
      global_MBR.magic = 42;
      global_MBR.size = 0;
    }


}




void save(){
  char buffer [HDA_SECTORSIZE];
  memcpy(buffer, &global_MBR, sizeof(struct mbr_ctx));
  write_sector(0,0,buffer);


}

int cyl_of_block(unsigned int vol,unsigned int nbblock){
  struct partition_ctx p = global_MBR.tab[vol];
  int sect_start = p.sect;
  int cyl_start = p.cyl;
  return cyl_start + ((nbblock + sect_start)/8);
}

int sect_of_block(unsigned int vol,unsigned int nbblock){
  struct partition_ctx p = global_MBR.tab[vol];
  int sect_start = p.sect;
  return (sect_start + nbblock)%8;
}

void read_block(unsigned int vol, unsigned int nbblock, unsigned char *buffer){
  unsigned int cyl = cyl_of_block(vol,nbblock);
  unsigned int sect = sect_of_block(vol,nbblock);
  read_sector(cyl,sect,buffer);
}
void write_block(unsigned int vol, unsigned int nbblock,const unsigned char *buffer){
  unsigned int cyl = cyl_of_block(vol,nbblock);
  unsigned int sect = sect_of_block(vol,nbblock);
  write_sector(cyl,sect,buffer);
}

void format1_block(unsigned int vol, unsigned int nbblock){
  unsigned int cyl = cyl_of_block(vol,nbblock);
  unsigned int sect = sect_of_block(vol,nbblock);
  format1_fmt(cyl,sect);
}
void format_vol(unsigned int vol){
  unsigned int i =0;
  unsigned int nbblock = global_MBR.tab[vol].nbblock;
  for (i;i < nbblock; i++) {
    format1_block(vol,i);
  }
}
