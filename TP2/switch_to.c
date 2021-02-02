#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "init_ctx.h"
#include "switch_to.h"

static struct ctx_s * context = NULL;
static struct ctx_s * current = NULL;

void switch_to_ctx(struct ctx_s *ctx){
  assert(ctx -> magic == MAGIC);
  assert(ctx -> state == READY || ctx -> state == ACTIVABLE);

  if(current){
    asm("movl %%esp, %0" "\n\t"   "movl %%ebp, %1"
        : "=r" (current->esp), "=r" (current->ebp)
        :
        :);
  }
  current = ctx;

  asm("movl %0,%%esp" "\n\t" "movl %1,%%ebp"
      :
      : "r" (current->esp), "r" (current->ebp)
      :);

  if(current -> state == READY){
    current -> state = ACTIVABLE;
    current -> func(current -> arguments);
    current -> state = TERMINATED;
    yield();
  }
}

int main(){
    create_ctx(16384, f_ping, NULL);
    create_ctx(16384, f_pong, NULL);
    yield();
    exit(EXIT_SUCCESS);
}

void f_ping(void *args){
      for (int i = 0; i < 20; i++) {
        printf("A") ;
        yield();
        printf("B") ;
        yield();
        printf("C") ;
        yield();
    }
}

void f_pong(void *args){
    for (int i = 0; i < 10; i++) {
        printf("1") ;
        yield();
        printf("2") ;
        yield();
    }
}

int create_ctx(int stack_size, func_t *f, void *args){
  struct ctx_s * new_ctx = malloc(sizeof(struct ctx_s));

  if (new_ctx == NULL) return 1;
  init_ctx(new_ctx,stack_size,f,args);
  if(context == NULL){
    new_ctx -> next = new_ctx;
    context = new_ctx;
  }else{
    new_ctx -> next = context -> next;
    context -> next = new_ctx;
  }
  printf("On a cree le contexte %x\n", new_ctx);
  return 0;
}

struct ctx_s * choose_next(){
  if(context == NULL){
    return NULL;
  }
  if (current == NULL){
    return context;
  }
  struct ctx_s * tmp = current;
  while(tmp -> next -> state == TERMINATED) {
    int dernier_contexte = (tmp->next == tmp);
    free(tmp -> next -> addr);
    struct ctx_s * bye = tmp -> next;
    tmp -> next = tmp -> next -> next;
    free(bye);
    printf("On a libre le contexte: %x\n", bye);
    if(dernier_contexte){
      return NULL;
    }
  }
  return tmp -> next;
}

void yield(){
  struct ctx_s *tmp;
  tmp = choose_next();
  if (tmp == NULL) {
    exit(0);
  }
  //printf("Lance le contexte: %x\n", tmp);
  switch_to_ctx (tmp);
}
