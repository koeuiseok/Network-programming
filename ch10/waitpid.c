#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    int status;

    pid = fork();
    if(pid == 0)
    {  
        sleep(15);
        return 24;
    }
    else
    {
        while(!waitpid(-1, &status, WNOHANG))
        {
            sleep(3);
            puts("I am working now\n");
        }

        if(WIFEXITED(status)){
            printf("Child send %d\n", WEXITSTATUS(status));
        }
    } 
}