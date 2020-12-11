#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREAD 100

long long num=0;

pthread_mutex_t mutex;

void *thread_inc (void *arg)
{
    int cnt_i;

    pthread_mutex_lock(&mutex);
    for(cnt_i =0; cnt_i <5000000; cnt_i++)
    {
        num +=1;
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void *thread_dec (void *arg)
{
    int cnt_i;

    
    for(cnt_i =0; cnt_i <5000000; cnt_i++)
    {
        pthread_mutex_lock(&mutex);
        num -=1;
        pthread_mutex_unlock(&mutex);
    }
    

    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t thread_id[NUM_THREAD];
    void* thr_ret = NULL;
    int cnt_i;

    pthread_mutex_init(&mutex, NULL);

    for(cnt_i =0; cnt_i<NUM_THREAD; cnt_i ++)
    {
        if(cnt_i%2){
            pthread_create(&thread_id[cnt_i],NULL, thread_inc, NULL);
        }
        else{
            pthread_create(&thread_id[cnt_i],NULL, thread_dec, NULL);
        }
        
    }

    for(cnt_i = 0; cnt_i <NUM_THREAD; cnt_i++)
    {
        pthread_join(thread_id[cnt_i], &thr_ret);
    }

    printf("result :  %lld\n",num);


    pthread_mutex_destroy(&mutex);

    return 0;
}