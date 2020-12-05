#include <stdio.h>
#include <unistd.h>

 int main()
 {
     pid_t pid;
    pid = fork();

    if(pid == 0){
        puts("Hi, I am a child process");
    }
    else{
        printf("Child process ID : %d \n", pid);
        sleep(30);
    }
    
    if(pid ==0){
        printf("end child process");
    }
    else{
        printf("end parent process");

    }    
     return 0;
 }