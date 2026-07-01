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
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    int start = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    usleep(2000000);
    gettimeofday(&tv, NULL);
    int end = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    printf("Elapsed time: %d ms\n", end - start);
    return 0;
}