#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int sleepUs = atoi(argv[1]);

    printf("Top test started\n");
    while (1)
    {
        usleep(sleepUs);
    }

    return 0;
}