#include "../include/utils.h"

#include <threads.h>

void sleep_ms(const uint16_t ms)
{
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    thrd_sleep(&ts, NULL);
}
