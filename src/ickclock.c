#include "ickclock.h"
ickclock_t* ickclock() {
  ickclock_t* this = malloc(sizeof(ickclock_t));
  thrd_t* cthread  = malloc(sizeof(thrd_t));
  if (!this || !cthread) { return NULL; }
  this -> clock_status = TC_IDLE;
  this -> clock_mode = TC_SEQ;
  this -> clock_thrd = cthread;
  this -> curr = 0;
  this -> clen = 0;
  this -> cend = 0;
  return this;
}
int ickclock_smash(ickclock_t** tar) {
  if ((*tar) -> clock_status == TC_JOB) { 
    ickclock_kill(*tar);
    ickclock_smash(tar);
  }
  if (!tar || !(*tar))
  { return -1; }
  if (!((*tar) -> clock_thrd))
  { return -1; }
  free((*tar) -> clock_thrd);
  (*tar) -> clock_thrd = NULL;
  free(*tar); (*tar) = NULL;
  return 0;
}
int ickclock_kill(ickclock_t* tar) {
  if (tar -> clock_status != TC_JOB) {
    return -1;
  }
  tar -> clock_status = TC_TERMINATE;
  if (tar -> clock_mode == TC_SEQ) {
    return 0;
  }
  return thrd_detach(*(tar -> clock_thrd));
}
int ickclock_boot(ickclock_t* tar) {
  if (thrd_create(tar -> clock_thrd, ickwait, tar) != thrd_success)
  { return -1; }
  tar -> clock_status = TC_JOB;
  tar -> curr = time(0);
  if (tar -> clock_mode != TC_SEQ)
  { return 0; }
  int exit_status = 0;
  if (thrd_join(*(tar -> clock_thrd), &exit_status) == thrd_success) {
    return 0;
  }
  return -1;
}
void ickclock_reset(ickclock_t* tar) {
  tar -> clock_status = TC_IDLE;
  tar -> clock_thrd = NULL;
  tar -> curr = time(0);
  tar -> cend = 0;
  tar -> clen = 0;
}
int ickwait(void* tar) {
  ickclock_t* clock = tar;
  time_t clock_len = clock -> clen;
  if (clock -> clock_mode == TC_TICK) {
    while (clock -> clock_status != TC_TERMINATE)
    { thrd_yield(); }
    clock -> cend = time(0);
    clock -> clen = clock -> cend - clock -> curr;
    return 0;
  }
  if (thrd_sleep(
    &(struct timespec) {
    .tv_sec = clock_len},
    NULL
  ) != thrd_success) { return -1; }
  clock -> cend = time(0);
  clock -> clen = clock_len;
  return ickclock_kill(clock);
}
