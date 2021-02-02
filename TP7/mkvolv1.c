#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "mbr.h"
#include "mkvolv1.h"
#include "hwconfig.h"
#include "hardware.h"
#define MAGIC 42
static struct mbr_ctx global_MBR;
static struct partition_ctx test_part;
void new_partition(int nbblock, int sect, int cyl,char type){
  assert(global_MBR.size < 8); /* MBR FULL*/

  _mask(15);
  int i;
  for ( i= 0; i < global_MBR.size; i++) {
    if((global_MBR.tab[i].sect + global_MBR.tab[i].nbblock)%8 >= sect || global_MBR.tab[i].cyl > cyl){
      printf("Attention chevauchement des partitions\n");
      return;
    }
  }
  struct partition_ctx new_mbr;
  new_mbr.nbblock = nbblock;
  new_mbr.sect = sect;
  new_mbr.cyl = cyl;
  switch (type) {
    case 'b': new_mbr.type = BASE;break;
    case 'a': new_mbr.type = ANNEXE;break;
    case 'c': new_mbr.type = OTHER;break;
    default : new_mbr.type = DEFAULT;
  }
  global_MBR.tab[global_MBR.size] = new_mbr;
  global_MBR.size += 1;
  save();
  _mask(1);
}

void delelete_partition(int vol){
  if(vol >= global_MBR.size){
    printf("numero de volume invalide \n");
    return;
  }
  assert(global_MBR.size > 0);/*MBR EMPTY */
  int i;
  _mask(15);
  global_MBR.size -= 1;

  for(i = vol; i< global_MBR.size; i++){
    global_MBR.tab[i] = global_MBR.tab[i + 1];
  }

  save();
  _mask(1);
}



void display_list(){
  if(global_MBR.size<=0){
    printf("MBR is empty\n");
    return;
  }
  _mask(15);
  int i;
  for(i = 0; i < global_MBR.size; i++){
    struct partition_ctx partition_current = global_MBR.tab[i];
    if(partition_current.type != DEFAULT){
      printf("- volume i %d\n",i);
      printf("--- secteur %d\n", partition_current.sect);
      printf("--- cycle %d\n", partition_current.cyl);
      printf("--- nbblock %d\n", partition_current.nbblock);
      printf("--- type %d\n", partition_current.type);
      printf("--- cylcle du blockk %d\n", cyl_of_block(i,partition_current.nbblock));
      printf("--- secteur du blockk %d\n", sect_of_block(i,partition_current.nbblock));
    }else{
      printf("none\n");
    }
  }
  _mask(1);
  return;
}

static void
empty_it()
{
   return;
}

void init_master(){
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
}
int main(int argc, char const *argv[]) {
  char buff[1024];
  _mask(15);
  init_master();
  load_mbr();
  printf("SALLLLLLLUT\n" );
  printf("%d\n",global_MBR.size );
  new_partition(6,2,3,'b');
  new_partition(6,3,3,'a');
  printf("%d\n",global_MBR.size );

  display_list();
  delelete_partition(1);
  display_list();
  printf("tu passes là?\n");
  exit(EXIT_SUCCESS);
}
