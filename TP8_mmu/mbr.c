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
static struct superblock_s global_superblock;

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


void init_volume(unsigned int vol){
  if(vol >= global_MBR.size || vol < 0){
    printf("taille volume non valide \n");
    return;
  }
  struct superblock_s superblock;
  char* buffer[BLOCKSIZE];
  superblock.magic = MAGIC;
  superblock.num=0;
  strcpy(superblock.name,"SUPERBLOCK");
  superblock.first_free_block = 1;
  superblock.nb_free_block = MAX_BLOCK -1;
  memcpy(buffer,&superblock,sizeof(struct superblock_s));
  write_block(vol,0,buffer);
  struct free_block_s fb;
  fb.size_block =MAX_BLOCK -1;
  fb.next_block = -1;
  memcpy(buffer,&fb,sizeof(struct free_block_s));
  write_block(vol,1,buffer);
}
int load_super(unsigned int vol){
  struct partition_ctx p = global_MBR.tab[vol];
  current_vol = vol;
  char buffer [BLOCKSIZE];
  memcpy(&global_superblock,buffer,sizeof(struct superblock_s));
  read_sector(p.cyl,p.sect,buffer);
  if(global_MBR.magic != MAGIC){
    init_volume(vol);
  }

}


void save_super(){
  char buffer [BLOCKSIZE];
  memcpy(buffer, &global_superblock, sizeof(struct superblock_s));
  write_block(current_vol,0,buffer);
}

unsigned new_bloc(){
  assert(global_superblock.nb_free_block >= 0);
  global_superblock.nb_free_block --;
  struct free_block_s *fb;
  char buffer[BLOCKSIZE];
  read_block(current_vol,global_superblock.nb_free_block,buffer);
  fb =  *buffer;
  if(fb->size_block>1){
    global_superblock.first_free_block ++;
    fb->size_block --;
    write_block(current_vol,global_superblock.nb_free_block,buffer);
  }
  else{
    global_superblock.first_free_block = fb->next_block;
  }

}

void free_block(int block){
  assert(global_superblock.nb_free_block <= MAX_BLOCK ); /* on vérfie qu'on ne peut pas libérer plus de bloc que le max possible */
  int oldfirst = global_superblock.first_free_block;
  global_superblock.first_free_block = block;
  global_superblock.nb_free_block ++;
  char buffer[BLOCKSIZE];
  struct free_block_s *fb =buffer;
  fb->size_block =1;
  fb->next_block = oldfirst;
  write_block(current_vol,global_superblock.nb_free_block,buffer);

}
