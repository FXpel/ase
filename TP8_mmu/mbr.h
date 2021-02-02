
#define MAGIC 42
#define HDA_SECTORSIZE 1024
#define HDA_MAXCYLINDER 16
#define HDA_MAXSECTOR 16
#define MAX_BLOCK HDA_MAXCYLINDER*HDA_MAXSECTOR
#define BLOCKSIZE 256

enum partType {BASE, ANNEXE, OTHER, DEFAULT};
struct partition_ctx{
  int nbblock;
  int sect;
  int cyl;
  enum partType type;};

struct mbr_ctx{
  struct partition_ctx tab[8];
  int size;
  int magic;};

typedef struct superblock_s{
  int magic;
  unsigned int num;
  char* name[32];
  /*char* inoeud*/
  int nb_free_block;
  int first_free_block;};

typedef struct free_block_s{
  int size_block;
  int next_block;};

void init_volume(unsigned int vol);
int load_super(unsigned int vol);
void load_mbr();

void save_mbr();

int cyl_of_block(unsigned int vol,unsigned int nbblock);
int sect_of_block(unsigned int vol,unsigned int nbblock);

void read_block(unsigned int vol, unsigned int nbblock, unsigned char *buffer);
void write_block(unsigned int vol, unsigned int nbblock,const unsigned char *buffer);
void format1_block(unsigned int vol, unsigned int nbblock);

void init_volume(unsigned int vol);
int load_super(unsigned int vol);
void save_super();
unsigned int new_bloc();
void free_block();
