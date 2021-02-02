void seek(unsigned short cyl, unsigned short sect);
void read_sector(unsigned int cylinder, unsigned int sector,
  unsigned char *buffer);
void write_sector(unsigned int cylinder, unsigned int sector,
                  const unsigned char *buffer);
void format_sector(unsigned int ncylinder, unsigned int nsector,
  unsigned int value);

void format1(unsigned int cylinder, unsigned int sector,unsigned int nsector ,
 unsigned int value);
#define HDA_IRQ 14
#define HDA_DATAREGS 0x110
#define HDA_CMDREGS 0x3F6
#define BUFFSIZE 256
#define NB_SECT 0x0001
#define SEEK_CMD 0x02
#define READ_CMD 0x04
#define WRITE_CMD 0x06
#define FORMAT_CMD 0x08
