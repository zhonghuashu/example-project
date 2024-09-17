#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

void busyCpu()
{
    struct timespec tpstart;
    struct timespec tpend;
    long timediff;
    static int isPrinted = false;

    clock_gettime(CLOCK_MONOTONIC, &tpstart);

    volatile long long i;
    for (i = 0; i < 3000; ++i)
    {
        // Loop 3000: 95 us / CPU 25 %
    }

    clock_gettime(CLOCK_MONOTONIC, &tpend);
    timediff = 1000 * 1000 * (tpend.tv_sec - tpstart.tv_sec) + (tpend.tv_nsec-tpstart.tv_nsec) / 1000;

    if (!isPrinted)
    {
        printf("Busy time: %ld us\n", timediff);
        isPrinted = true;
    }

}

void posixTimerSignal(int sig, siginfo_t *si, void *pri)
{
    static int i = 0;
    static int count = 0;

    busyCpu();
    if (i++ == 2000)
    {
        printf("posix_timer: %d\n", count++);
        i = 0;
    }
}

void posixTimerTest()
{
    timer_t timer = {0};
    struct sigevent sev = {0};
    struct sigaction sa = {0};

    const struct timespec timeVal = {
          .tv_sec = 0,
          .tv_nsec = 500 * 1000 // 500 us
    };

    const struct itimerspec its =
    {
        .it_interval = timeVal,
        .it_value = timeVal
    };

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGRTMIN;
    sev.sigev_value.sival_ptr = &timer;

    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = posixTimerSignal;

    if(sigemptyset(&sa.sa_mask) == -1)
    {
        perror("sigemptyset error");
    }

    if(sigaction(SIGRTMIN, &sa, NULL) == -1)
    {
        perror("sigaciton error");
    }

    if(timer_create(CLOCK_REALTIME, &sev, &timer) != 0)
    {
        perror("timer_create error");
    }

    if(timer_settime(timer, 0, &its, NULL) != 0)
    {
        perror("timer_settime error");
    }

    while (1)
    {
        pause();
    }
}

int main(int argc, char* argv[])
{
    printf("Timer test started\n");
    posixTimerTest();
    return 0;
}