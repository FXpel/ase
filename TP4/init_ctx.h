#define MAGIC 0x42424242

typedef void func_t(void*);
enum statut {READY, ACTIVABLE,TERMINATED,BLOCKED};

typedef struct ctx_s {
  void * esp;
  void * ebp;
  int magic;
  char *addr;
  enum statut state;
  func_t *func;
  void *arguments;
  struct ctx_s *next;
  struct ctx_s *next_sem;
}ctx_s;

int init_ctx(struct ctx_s *ctx, int stack_size,func_t *f, void *args);
