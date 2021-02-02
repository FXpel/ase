#define N 4096
#define TLB_ADD_ENTRY 0xCE
#define MMU_FAULT_ADDR 0XCD
#define TLB_DEL_ENTRY	0xDE
#define NUM_VIRT 4096
#define PAGE_SIZE 4096
#define NUM_PHYS 256
#define PHYSPAGE 2
#define MMU_IRQ 13


struct tlb_entry_s{
  char rfu;
  short int pv:12;
  char pp;
  char exec:1;
  char write:1;
  char read:1;
  char used:1};

int store_to_swap(int vpage, int ppage);
int fetch_from_swap(int vpage, int ppage);
void handler();
static int init_swap(void);
int ppage_to_vpapge(int ppage);
int vpage_to_ppage(int vpage);
int new_page(int vpage, int *old_vpage);
void mmu_handler();
void init_master();
static int ppage_of_vpage(int process,unsigned vpage);
static int ppage_of_vpage2(int process,unsigned vpage);
void handler();
