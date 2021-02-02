
#define MAGIC 42
#define HDA_SECTORSIZE 1024

enum partType {BASE, ANNEXE, OTHER, DEFAULT};
struct partition_ctx{
  int nbblock;
  int sect;
  int cyl;
  enum partType type;
};

struct mbr_ctx{
  struct partition_ctx tab[8];
  int size;
  int magic;
};


void init_volume(unsigned int vol);
int load_super(unsigned int vol);
void load_mbr();

void save_mbr();

int cyl_of_block(unsigned int vol,unsigned int nbblock);
int sect_of_block(unsigned int vol,unsigned int nbblock);

void read_block(unsigned int vol, unsigned int nbblock, unsigned char *buffer);
void write_block(unsigned int vol, unsigned int nbblock,const unsigned char *buffer);
void format1_block(unsigned int vol, unsigned int nbblock);
