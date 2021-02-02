#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "init_ctx.h"
#include "start_schedule.h"
#include "hwconfig.h"
#include "hardware.h"
#include <unistd.h>


static struct ctx_s * context = NULL;
static struct ctx_s * current = NULL;

void switch_to_ctx(struct ctx_s *ctx){
  irq_disable();
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
  irq_enable();
}

int create_ctx(int stack_size, func_t *f, void *args){
  irq_disable();
  struct ctx_s * new_ctx = malloc(sizeof(struct ctx_s));

  if (new_ctx == NULL){
    irq_enable();
    return 1;
  }
  init_ctx(new_ctx,stack_size,f,args);
  if(context == NULL){
    new_ctx -> next = new_ctx;
    context = new_ctx;
  }else{
    new_ctx -> next = context -> next;
    context -> next = new_ctx;
  }
  printf("On a cree le contexte %x\n", new_ctx);
  irq_enable();

  return 0;
}

struct ctx_s * choose_next(){
  irq_disable();
  if(context == NULL){
    irq_enable();
    return NULL;
  }
  if (current == NULL){
    irq_enable();
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
      irq_enable();
      return NULL;
    }
  }
  irq_enable();
  return tmp -> next;
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
int main(){
    create_ctx(16384, f_ping, NULL);
    create_ctx(16384, f_pong, NULL);
    start_sched();
    while(1){
      pause();
    }

    exit(EXIT_SUCCESS);
}

void yield(){
  struct ctx_s *tmp;
  tmp = choose_next();
  if (tmp == NULL) {
    exit(0);
  }
  _out(TIMER_ALARM,0xFFFFFFFE);
  //printf("Lance le contexte: %x\n", tmp);
  switch_to_ctx (tmp);
}



static void empty_it(void){
  return;
}

void start_sched(){
  int i;
  if (init_hardware(INIFILENAME) == 0) {
    fprintf(stderr, "Error in hardware initialization\n");
    exit(EXIT_FAILURE);
  }
  /* dummy interrupt handlers */
  for (i=0; i<16; i++){
    IRQVECTOR[i] = empty_it;
  }
  IRQVECTOR[TIMER_IRQ] = yield;
  _out(TIMER_PARAM,0xE8);
  _out(TIMER_ALARM,0xFFFFFFFE);
  irq_enable();
}

void irq_disable(){
  _mask(15);
}

void irq_enable(){

  _mask(1);
}
