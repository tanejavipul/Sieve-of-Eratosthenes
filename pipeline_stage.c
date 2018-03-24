#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <math.h>

#include "eratosthenes.h"


pid_t make_stage(int m, int read_fd, int **fds)
{
    int ret;
    
    //int &status;
    pipe(fds[0]);
    ret = fork();
    if(ret == 0) //child
    {
        close(fds[0][1]);
        return 0; //fix
    }
    else if(ret > 0) //parent
    {
        close(fds[0][0]);
        filter(m,read_fd,fds[0][1]);
        close(fds[0][1]);
        return ret;
    }
    else
    {
        exit(255);
    }
}
