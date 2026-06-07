#ifndef ICKLIB_H_ICKCLOCK
#define ICKLIB_H_ICKCLOCK
#include <threads.h>
#include <stdlib.h>
#include <time.h>
typedef enum {
    TC_ERR        = -1,
    TC_IDLE       = 0,
    TC_JOB        = 1,
    TC_TERMINATE  = 2,
} ickclock_stat;
typedef enum {
    TC_THRD       = 0,
    TC_SEQ        = 1,
    TC_TICK       = 2,
} ickclock_md;
typedef struct {
  ickclock_stat clock_status;
  ickclock_md   clock_mode;
  thrd_t* clock_thrd;
  time_t curr;
  time_t cend;
  time_t clen;
} ickclock_t;
ickclock_t* ickclock();
int ickclock_smash(ickclock_t** tar);
int ickclock_kill(ickclock_t* tar);
int ickclock_boot(ickclock_t* tar);
void ickclock_reset(ickclock_t* tar);
static inline void ickclock_set(ickclock_t* tar, int len)
{ tar -> clen = len; }
static inline void ickclock_mode(ickclock_t* tar, ickclock_md md)
{ tar -> clock_mode = md; }
int ickwait(void* tar);
#endif
