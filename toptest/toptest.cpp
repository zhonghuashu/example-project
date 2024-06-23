#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int sleepUs = ::atoi(argv[1]);

    std::cout << "Top test started" << std::endl;
    while (true)
    {
        ::usleep(sleepUs);
    }

    return 0;
}