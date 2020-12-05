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
        return 3;
    }
    else
    {
        printf("child PID : %d\n", pid);
        pid = fork();

        if(pid == 0){
            exit(7);
        }
        else{
            wait(&status);
            if(WIFEXITED(status)){
                 printf("child send one : %d\n", WEXITSTATUS(status));
            }

            wait(&status);
            if(WIFEXITED(status)){
                 printf("child send one : %d\n", WEXITSTATUS(status));
            }
            sleep(30);
        }
    } 
}