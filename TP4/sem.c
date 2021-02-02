#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include "init_ctx.h"
#include "sem.h"
#include "hwconfig.h"
#include "hardware.h"

static struct ctx_s * current = NULL;

void sem_init(struct sem_s *sem, unsigned int val){
  sem -> sem_wait = NULL;
  sem -> cpt = val;
}


void sem_down(struct sem_s *sem){
  sem -> cpt--;
  if(sem -> cpt <0){
    current -> state = BLOCKED;

    if (sem -> sem_wait == NULL){
      sem -> sem_wait = current;
    }
    else{
      sem -> sem_wait -> next_sem = current;
    }
    yield();

  }
}

void sem_up(struct sem_s * sem){
  sem -> cpt ++;
  if(sem -> cpt <= 0){
    struct ctx_s * temp;
    temp = sem -> sem_wait;
    temp ->state = ACTIVABLE;
    sem -> sem_wait = temp -> next_sem ;
    temp -> next_sem = NULL;
  }




}
