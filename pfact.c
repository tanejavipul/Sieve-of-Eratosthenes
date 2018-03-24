#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <math.h>

#include "eratosthenes.h"

int main(int argc, char *argv[]) {
    // Turning off sigpipe
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("signal");
        exit(1);
    }

    if(argc != 2)
    {
        fprintf(stderr, "Usage:\n\tpfact n\n");
        exit(1);
    }

    int fd[2], m = 2;
    int factor[2], filter_counter = 0;
    int number = strtol(argv[1],NULL,10);
    int num_count = 0;
    int temp_num = number;
    while(temp_num != 0)
    {
        // n = n/10
        temp_num /= 10;
        ++num_count;
    }


    if(number < 2 || num_count != strlen(argv[1]))
    {
        fprintf(stderr, "Usage:\n\tpfact n\n");
        exit(1);
    }
    
    
    if(number%(m*m) == 0)
    {
        if((2*2)%number != 0)
        {
            printf("%d is not the product of two primes\n",number);
            printf("Number of filters = %d\n",filter_counter);
            exit(0);
        }

    }


    pipe(fd);
    int input_fork;
    input_fork = fork();

    if(input_fork > 0)
    {
        for(int x = 2;x<(number/2)+2;x++) {
            write(fd[1], &x, sizeof(int));
        }
        close(fd[1]);
        close(fd[0]);
        wait(&input_fork);
        exit(0);
    }

    close(fd[1]);


    //int status;
    int test_m = 0;
    int total_factors=0;
    int reader = fd[0];
   // int last_m;


    while(m <= sqrt(number) && total_factors < 2) //total_factors
    {

        int temp[2];
        int *fds[1];
        fds[0] = temp;

        if((number%m) == 0)
        {
            if(total_factors < 2)
            {
                factor[total_factors] = m;
            }
            total_factors++;
        }
        if(number%(m*m) == 0)
        {
            if (total_factors < 2) {
                factor[total_factors] = m;
            }
            total_factors++;
            //last_m = m;
        }


        if(total_factors < 2) //total_factors
        {
            int fork_ret = make_stage(m,reader,fds);
            filter_counter++;
            if(fork_ret==0)
            {
                read(fds[0][0],&m,sizeof(int));
                if(m > sqrt(number))
                {
                    test_m = 1;
                }
                reader = fds[0][0];
            }
            if(fork_ret > 0)
            {
                wait(&fork_ret);
                exit(0);
            }
            if(fork_ret < 0)
            {
                close(reader);
                exit(2);
            }
            
        }


    }

    if(test_m == 1 && total_factors <2)
    {
        if(number%m == 0 && total_factors < 2)
        {
            if(total_factors < 2)
            {
                factor[total_factors] = m;
            }
            total_factors++;
        }
    }


    while(read(reader,&m,sizeof(int)) == sizeof(int) && total_factors <2)
    {
        if(number%m == 0 && total_factors < 2)
        {
            if(total_factors < 2)
            {
                factor[total_factors] = m;
            }
            total_factors++;
        }
    }
    
    
    
    if((factor[0]*factor[1])%number != 0)
    {
        printf("%d is not the product of two primes\n",number);
        printf("Number of filters = %d\n",filter_counter);
        close(reader);
        exit(0);
    }
    else {
        if (total_factors == 2) {
            printf("%d %d %d\n", number, factor[0], factor[1]);
            printf("Number of filters = %d\n", filter_counter);
            close(reader);
            exit(0);
        } else {
            printf("%d is prime\n", number);
            printf("Number of filters = %d\n", filter_counter);
            close(reader);
            exit(0);
        }
    }
    close(reader);
    exit(0);
}
