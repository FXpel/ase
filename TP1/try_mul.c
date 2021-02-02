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

static int mul(int depth) {
    int i;
    switch (scanf("%d", &i)) {
        case EOF :
            return 1; /* neutral element */
        case 0 :
            return mul(depth+1); /* erroneous read */
        case 1 :
            if (i){
              printf("mul effectué\n");
              return i * mul(depth+1);
            }else{
              throw(&c,0);
            }
    }
}

int main(){
    int r;
    r = try(&c, mul, 0);
    printf("resultat : %d\n", r);
}
