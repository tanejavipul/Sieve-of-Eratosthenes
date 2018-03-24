#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <math.h>

#include "eratosthenes.h"


int filter(int m, int readfd, int writefd)
{
    int incoming;
    int ret = 0;
    while(read(readfd,&incoming,sizeof(int)) == sizeof(int))
    {
        if(incoming%m != 0)
        {
            if(write(writefd,&incoming,sizeof(int)) == -1)
            {
                ret = 1;
            }
        }
    }

    return ret;
}





