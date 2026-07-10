#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

long long counter = 0;
pthread_mutex_t mutex;
pthread_cond_t  conc;

void* WORKER_SAY(void* i)
{
   
}
void* SIGNALER_SAY(void* i)
{
    
}

int main()
{
    pthread_t worker;
    pthread_t signaler;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&conc, NULL);
    pthread_create(&worker, NULL, WORKER_SAY, NULL);
    pthread_create(&signaler, NULL, SIGNALER_SAY, NULL);

    pthread_join(worker, NULL);
    pthread_join(signaler, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&conc);

    return 0;
}