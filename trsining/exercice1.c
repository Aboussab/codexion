#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

long long counter = 0;
pthread_mutex_t mutex;
pthread_cond_t  conc;

void* MAIN_SAY(void* i)
{
    // usleep(3000000);
    pthread_mutex_lock(&mutex);
    printf("Wake up!");
    counter++;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&conc);
    return NULL;
}
void* WORKER_SAY(void* i)
{
    pthread_mutex_lock(&mutex);
    printf("Worker: I'm waiting...");
    while (counter == 0)
        pthread_cond_wait(&conc, &mutex);
    printf("I'm awake!");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main()
{
    pthread_t   main;
    pthread_t   worker;

    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&conc, &mutex);
    pthread_create(&worker, NULL, WORKER_SAY, NULL);
    pthread_create(&main, NULL, MAIN_SAY, NULL);
    pthread_join(&worker, NULL);
    pthread_join(&main,NULL);
     pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&conc);
    return 0;
}