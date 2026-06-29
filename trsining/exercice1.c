#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

long long counter = 0;
pthread_mutex_t mutex1;


void* hello_thread(void* i)
{
    printf("thread %i is working\n",*(int *)i);
    return NULL;
}


int main()
{
    pthread_t   thread1;
    pthread_t   thread2;
    pthread_t   thread3;


    int start = gettimeofday();
    pthread_mutex_init(&mutex1, NULL);
    int i1 = 1;
    int i2 = 2;
    int i3 = 3;
    pthread_mutex_lock(&mutex1);

    pthread_create(&thread1,NULL,hello_thread,&i1);
    pthread_create(&thread2,NULL,hello_thread,&i2);
    pthread_create(&thread3,NULL,hello_thread,&i3);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_mutex_unlock(&mutex1);
    struct timeval start, end;
    gettimeofday(&start, NULL);
    usleep(20000);

    // ... run your code or usleep() here ...

    gettimeofday(&end, NULL);
    // Convert everything to milliseconds
    long long end_time = (end.tv_sec * 1000LL) + (end.tv_usec / 1000);

    printf("Elapsed time: %lld ms\n", end_time - start_time);   
    return 0;
}