#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

void *thread_main (void *arg)
{
    int cnt_i;
    int limit = *((int*)arg);
    char *msg = (char*)malloc(64*sizeof(char));
    strncpy(msg, "Jello, I am thread\n",64);

    for(cnt_i=0; cnt_i<limit; cnt_i++)
    {
        sleep(1);
        printf("running thread\n");
    }
    return msg;
}

int main(int argc, char* argv[])
{
    pthread_t t_id;
    int thread_param = 5;
    
    void* thr_ret = NULL;

    if(pthread_create(&t_id, NULL, thread_main, (void*)&thread_param)!=0)
    {
        fprintf(stderr,"pthread_create() error\n");
        return -1;
    }
    if(pthread_join(t_id, &thr_ret)!=0)
    {
        fprintf(stderr,"thread_join() error");
        return -1;
    }
    
    printf("thread return message :  %s\n",(char*)thr_ret);

    if(thr_ret !=NULL)
    {
        free(thr_ret);
    }

    return 0;
}