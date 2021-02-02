typedef struct sem_s {
  int cpt;
  struct ctx_s *sem_wait;
}sem_s;




void init_sem();
void sem_down();
void sem_up();
