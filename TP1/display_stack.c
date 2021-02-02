#include <setjmp.h>
#include <stdio.h>

static int i = 0;
static jmp_buf buf;

#define ASM {\
      asm ("movl %%esp, %0" "\n\t" "movl %%ebp, %1"\
      : "=r"(x), "=r"(y));\
    };

static int x,y;

int foo(){
  ASM
  printf("x=%d || y=%d \n",x,y);
  int j;
  if (setjmp(buf)){
    for (j=0; j<5; j++){
      i++;
    }
  }
  else {
    for (j=0; j<5; j++){
      i--;
    }
    printf("j=%d\n", &j);
    longjmp(buf,~0);
  }
}

int main(void){
  ASM
  printf("x=%d || y=%d \n",x,y);
  foo();
  ASM
  printf("x=%d || y=%d \n",x,y);
}
