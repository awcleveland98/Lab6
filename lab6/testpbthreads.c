//testpbthreads.c
#include <time.h>

double tvzero;

double tvgetf(void)
{
    struct timespec ts;
    double sec;

    clock_gettime(CLOCK_REALTIME,&ts);
    sec = ts.tv_nsec;
    sec /= 1e9;
    sec += ts.tv_sec;

    sec -= tvzero;

    return sec;
}
