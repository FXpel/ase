#define MAGIC 0x42424242

typedef struct ctx_s{
  int esp;
  int ebp;
  int magic;
}ctx_s;

static struct ctx_s c;

typedef int (* funct_t)(int);

int try(struct ctx_s *c, funct_t f, int arg);
int throw(struct ctx_s *c, int r);
static int mul(int depth);
