#ifndef __IO_BENCH
#define __IO_BENCH

#include <stdint.h>


typedef struct {
    uint64_t nbytes;
    long time;
    float rate;
} io_stat; 



#endif