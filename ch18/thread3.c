#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

int sum;

void *thread_sum (void *arg)
{
    int start = ((int*)arg)[0];
    int end = ((int*)arg)[1];

    while(start <= end)
    {
        sum +=start; 
        start++;
    }

    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t id_t1, id_t2;
    int range1[] = {1,5};
    int range2[] = {6,10};
    
    void* thr_ret = NULL;

    if(pthread_create(&id_t1, NULL, thread_sum, (void*)range1) !=0)
    {
        fprintf(stderr,"pthread2_create() error\n");
        return -1;
    }

    if(pthread_create(&id_t2, NULL, thread_sum, (void*)range2) !=0)
    {
        fprintf(stderr,"pthread2_create() error\n");
        return -1;
    }

    if(pthread_join(id_t1, &thr_ret)!=0)
    {
        fprintf(stderr,"thread1_join() error");
        return -1;
    }
    
    if(pthread_join(id_t2, &thr_ret)!=0)
    {
        fprintf(stderr,"thread2_join() error");
        return -1;
    }

    printf("result :  %d\n",sum);

    return 0;
}