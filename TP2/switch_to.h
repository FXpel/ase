void f_ping(void *arg);
void f_pong(void *arg);
void switch_to_ctx(struct ctx_s *ctx);
int create_ctx(int stack_size, func_t f, void *args);
void yield();
