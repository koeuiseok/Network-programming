#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
    if(sig == SIGALRM)
    {
        puts("Time out!");
    }

    alarm(2);


}

void keycontrol(int sig)
{
    if(sig == SIGINT)
    {
        puts("CTR+C pressed!");
    }
}


int main(int argc, char* argv[])
{
    int cnt_i;
    struct sigaction act;
    // signal(SIGALRM, timeout);

    act.sa_handler = timeout;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGALRM, &act, 0);
    alarm(2);

    signal(SIGINT, keycontrol);
    alarm(2);


    sigaction(SIGALRM, &act, 0);
    for(cnt_i =0; cnt_i <3; cnt_i++)
    {
        printf("wait...\n");
        sleep(100);
    }
    
    return 0;
}