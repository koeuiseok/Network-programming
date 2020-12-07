#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void read_childproc(int sig)
{
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if(WIFEXITED(status))
    {
        printf("Removed proc id : %d\n", pid);
        printf("Child send %d\n", WEXITSTATUS(status));
    }
}

int main()
{
    pid_t pid;
    int status;
    int cnt_i;
    struct sigaction act;

    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if(pid == 0)
    {
        printf("hi I am a child process\n");
        sleep(10);
        return 3;
    }
    else
    {
        printf("child PID : %d\n", pid);
        pid = fork();

        if(pid == 0)
        {
            printf("hi I am a child process\n");
            sleep(15);
            exit(7);
        }
        else
        {
            printf("child PID : %d\n", pid);
            for(cnt_i =0; cnt_i <8; cnt_i ++)
            {
                puts("wait!");
                sleep(5);
            }
        }
            sleep(30);
    } 
}