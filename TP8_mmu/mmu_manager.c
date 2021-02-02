#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "mmu_manager.h"
#include "hwconfig.h"
#include "hardware.h"

static FILE *swap_file;
int pm_mapping[NUM_VIRT];
int vm_mapping[NUM_PHYS];
int current_vpage;
int current_process;
struct tlb_entry_s global_tlb;

static int init_swap(void){
    swap_file = fopen(".swap_file", "w+"); /* w+: create, read, write*/
    return swap_file == NULL;
}
int store_to_swap(int vpage, int ppage){
    if (swap_file == NULL)
        if (init_swap())
            return -2;
    if (fseek(swap_file, vpage << 12, SEEK_SET) == -1)
        return -1;
    if (fwrite((void*)((ppage << 12) | (__intptr_t)physical_memory),
               1, PAGE_SIZE, swap_file) == 0)
        return -1;
  return 0;
}
int fetch_from_swap(int vpage, int ppage){
    if (swap_file == NULL)
        if (init_swap())
            return -2;
    if (fseek(swap_file, vpage << 12, SEEK_SET) == -1)
        return -1;
    if (fread((void*)((ppage << 12) | (__intptr_t)physical_memory),
	      1, PAGE_SIZE, swap_file) == 0)
        return -1;
    return 0;
}
int ppage_to_vpage(int ppage){
  return vm_mapping[ppage];
}
int vpage_to_ppage(int vpage){
  return pm_mapping[vpage];
}

int new_page(int vpage, int *old_vpage){
  int static cpt =1;
  int res = cpt;
  *old_vpage = vm_mapping[cpt];
  pm_mapping[vpage] = cpt;
  pm_mapping[*old_vpage] = -1;
  vm_mapping[cpt] = vpage;
  cpt ++;
  if (cpt >= NUM_PHYS) {
    cpt = 1;
  }
  return res;
}
void mmu_handler(){
  int addr = _in(MMU_FAULT_ADDR);
  int vpage = addr/4096;
  int ppage = ppage_to_vpage(vpage);
  global_tlb.rfu =0;
  global_tlb.pv = vpage;
  global_tlb.pp = ppage;
  global_tlb.exec = 1;
  global_tlb.write = 1;
  global_tlb.read = 1;
  global_tlb.used = 1;
  int entry;
  memcpy(&entry,&global_tlb,4);
  _out(TLB_ADD_ENTRY,entry);
}
static void empty_it()
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
 IRQVECTOR[MMU_IRQ] = mmu_handler;

  /* Allows all IT */
  _mask(0x1001);
}
static int ppage_of_vpage(int process,unsigned vpage){
  if (vpage > N/2){
    int ppage = (vpage * 2) + process;
    return ppage;
  }
  return -1;
}
static int ppage_of_vpage2(int process,unsigned vpage){
  assert(vpage <= N/2);
  int middle = N/2;
  if (process ==0)
    return vpage;
  return vpage + middle;
}

void handler(){
  int vpage = _in(MMU_FAULT_ADDR)/4096;
  int ppage = vpage_to_ppage(vpage);
  if(ppage == -1){
    int old;
    ppage = new_page(vpage,&old);
    if(old != -1)
      store_to_swap(old,ppage);
    fetch_from_swap(vpage,ppage);
  }
  add_tlb(vpage,ppage);
}

void add_tlb(int vpage,int ppage){
  global_tlb.rfu =0;
  global_tlb.pv = vpage;
  global_tlb.pp = ppage;
  global_tlb.exec = 1;
  global_tlb.write = 1;
  global_tlb.read = 1;
  global_tlb.used = 1;
  int entry;
  memcpy(&entry,&global_tlb,4);
  _out(TLB_ADD_ENTRY,entry);
}

int main(int argc, char const *argv[]) {
  init_master();
  IRQVECTOR[MMU_IRQ] = mmu_handler;

  return 0;
}
