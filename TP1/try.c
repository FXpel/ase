#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "try.h"

static int sr;

int try(struct ctx_s *c, funct_t f, int arg){
  asm("movl %%esp, %0" "\n\t"   "movl %%ebp, %1"
      : "=r" (c->esp), "=r" (c->ebp)
      :
      :);
  c->magic = MAGIC;
  printf("try\n");
  return f(arg);
}

int throw(struct ctx_s *c, int r){
  assert(c->magic == MAGIC);
  sr = r;
  asm("movl %0,%%esp" "\n\t" "movl %1,%%ebp"
      :
      : "r" (c->esp), "r" (c->ebp)
      :);
  printf("throw\n");
  return sr;
}
