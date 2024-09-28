/**
 * @file timer_test.cpp
 * @brief Timer test to verify timer cause high CPU load (see post in stack overflow:
 https://stackoverflow.com/questions/79010823/linux-posix-timer-cause-high-total-cpu-load)
 * @author Shu, Zhong Hua
 * @date 2024-09-28
 */
#include <csignal>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <getopt.h>

namespace {

struct TestParameter
{
    int loop;
    int timeUs;
    char type;
};

TestParameter mTestParameter = { 9000, 500 };

void busyCpu()
{
    timespec tpstart;
    timespec tpend;
    long timediff;
    static int isPrinted = false;

    ::clock_gettime(CLOCK_MONOTONIC, &tpstart);

    volatile long long i;
    for (i = 0; i < mTestParameter.loop; ++i)
    {
        // Loop 3000: 95 us / CPU 25 %
        // Loop 9000: 93 us / CPU 23 %  (Linux arrow-sockit 5.10.100-altera)
    }

    ::clock_gettime(CLOCK_MONOTONIC, &tpend);
    timediff = 1000 * 1000 * (tpend.tv_sec - tpstart.tv_sec) + (tpend.tv_nsec-tpstart.tv_nsec) / 1000;

    if (!isPrinted)
    {
        ::printf("Busy CPU time: %ld us with loop %d\n", timediff, mTestParameter.loop);
        isPrinted = true;
    }

}

void usleepTimeTest()
{
    int count = 0;
    int i = 0;
    ::printf("Start usleep timer test\n");
    const struct timespec ts = {
          .tv_sec = 0,
          .tv_nsec = mTestParameter.timeUs * 1000
    };

    while(true)
    {
        // ::usleep(500);
        ::nanosleep(&ts, nullptr);
        busyCpu();
        if (i++ == 2000)
        {
            ::printf("usleep count: %d\n", count);
            i = 0;
            count++;
        }
    }
}

void iTimerTestSignal(int signum)
{
    static int i = 0;
    static int count = 0;

    busyCpu();
    if (i++ == 2000)
    {
        ::printf("itimer: %d\n", count++);
        i = 0;
    }
}

void iTimerTest()
{
    const struct timeval timeVal = {
          .tv_sec = 0,
          .tv_usec = mTestParameter.timeUs
    };

    const itimerval its =
    {
        .it_interval = timeVal,
        .it_value = timeVal
    };

    ::signal(SIGALRM, iTimerTestSignal);

    if (::setitimer(ITIMER_REAL, &its, nullptr) == -1)
    {
        ::perror("setitimer failed");
    }

    while (true)
    {
        ::sleep(1);
    }

}

void posixTimerSignal(int sig, siginfo_t *si, void *pri)
{
    static int i = 0;
    static int count = 0;

    busyCpu();
    if (i++ == 2000)
    {
        ::printf("posix_timer: %d\n", count++);
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
          .tv_nsec = mTestParameter.timeUs * 1000
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

    if(::sigemptyset(&sa.sa_mask) == -1)
    {
        ::perror("sigemptyset error");
    }

    if(::sigaction(SIGRTMIN, &sa, nullptr) == -1)
    {
        ::perror("sigaciton error");
    }

    if(::timer_create(CLOCK_REALTIME, &sev, &timer) != 0)
    {
        ::perror("timer_create error");
    }

    if(::timer_settime(timer, 0, &its, nullptr) != 0)
    {
        ::perror("timer_settime error");
    }

    while (true)
    {
        ::pause();
    }

}

void selectTimeTest()
{
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = mTestParameter.timeUs;
    static int i = 0;
    static int count = 0;

    while (true)
    {
        if (0 == ::select(0, nullptr, nullptr, nullptr, &tv))
        {
            busyCpu();
            if (i++ == 2000)
            {
                ::printf("select_time: %d\n", count++);
                i = 0;
            }
        }
    }
}

void printUsage()
{
    ::printf("Usage: timer_test [options]\n");
    ::printf("Options: \n");
    ::printf(" -h, --help                    print help info\n");
    ::printf(" -u, --usleep                  using usleep timer\n");
    ::printf(" -i, --itimer                  using itimer\n");
    ::printf(" -p, --ptimer                  using posix timer\n");
    ::printf(" -s, --select                  using select timer\n");
    ::printf(" -l, --loop                    simulate busy CPU using loop with count, default 9000: 93 us / CPU 23 percentage\n");
    ::printf(" -t, --time                    time interval in us, default 500\n");
}

bool parseArgs(const int argc, char *argv[])
{
    bool ret = true;
    int opt;
    const int OPT_ARG_LEN = 10;
    const struct option longopts[] =
    {
        {"help",        0, nullptr, 'h'},
        {"usleep",      0, nullptr, 'u'},
        {"itimer",      0, nullptr, 'i'},
        {"ptimer",      0, nullptr, 'p'},
        {"select",      0, nullptr, 's'},
        {"loop",        1, nullptr, 'l'},
        {"time",        1, nullptr, 't'},
        {nullptr,       0, nullptr, 0}
    };

    if (argc < 2)
    {
        ::printf("Program need an argument\n");
        return false;
    }

    while ((opt = ::getopt_long(argc, argv, ":huipsl:t:", longopts, nullptr)) != -1)
    {
        switch (opt)
        {
            case 'h':
                printUsage();
                break;
            case 'u':
            case 'i':
            case 'p':
            case 's':
                mTestParameter.type = opt;
                break;
            case 'l':
                mTestParameter.loop = ::strtol(::optarg, nullptr, OPT_ARG_LEN);
                break;
            case 't':
                mTestParameter.timeUs = ::strtol(::optarg, nullptr, OPT_ARG_LEN);
                break;                                
            case ':':
                ::printf("Option needs a value\n");
                ret = false;
                break;
            case '?':
                ::printf("Unknown option: %c\n", ::optopt);
                ret = false;
                break;
            default:
                printUsage();
                ret = false;
                break;
        }
    }

    return ret;
}

}

int main(int argc, char *argv[])
{

    if (!parseArgs(argc, argv))
    {
        return EXIT_FAILURE;
    }

    ::printf("Time interval: %d\n", mTestParameter.timeUs);
    
    switch (mTestParameter.type)
    {
        case 'u':
            usleepTimeTest();
            break;
        case 'i':
            iTimerTest();
            break;
        case 'p':
            posixTimerTest();
            break;
        case 's':
            selectTimeTest();
            break;
        default:
            break;
    }

    return EXIT_SUCCESS;
}
