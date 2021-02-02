#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "init_ctx.h"

int init_ctx(struct ctx_s *ctx, int stack_size, func_t *f, void *args){
  ctx -> addr = malloc(stack_size);
  assert(ctx->addr);

  ctx -> state = READY;
  ctx -> esp = (ctx->addr) + stack_size -4;
  ctx -> ebp = (ctx->addr) + stack_size -4;
  ctx -> magic = MAGIC;
  ctx -> func = f;
  ctx -> arguments = args ;
  
  return EXIT_SUCCESS;
}
