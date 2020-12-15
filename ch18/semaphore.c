#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int num;
sem_t sem_one;
sem_t sem_two;

void* read_action(void *arg)
{
    int cnt_i;
    for(cnt_i=0; cnt_i < 5; cnt_i ++)
    {
        fputs("Input num ", stdout);
        sem_wait(&sem_two);    //sem2 1 -> 0
        scanf("%d", &num);
        sem_post(&sem_one);    //sem1 0 -> 1
    }
}

void* accu_action(void *arg)
{
    int sum =0, cnt_i;
    for(cnt_i=0; cnt_i<5; cnt_i ++)
    {
        sem_wait(&sem_one);    //sem_one 0 -> wait -> 1 -> 0
        sum += num;
        sem_post(&sem_two);    // sem two 0 -> 1
    }
    printf("Result : %d\n", sum);
}

int main(int argc, char* argv[])
{
    pthread_t id_t1, id_t2;

    sem_init(&sem_one,0,0);
    sem_init(&sem_two,0,1);

    pthread_create(&id_t1, NULL, read_action, NULL);
    pthread_create(&id_t2, NULL, accu_action, NULL);

    pthread_join(id_t1, NULL);
    pthread_join(id_t1, NULL);
    
    sem_destroy(&sem_one);
    sem_destroy(&sem_two);
}