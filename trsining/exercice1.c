#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

long long counter = 0;

void* hello_thread(void* i)
{
    printf("Hello, I am thread %d \n", *(int*)i+1);
    for(int j = 0;j < 100000; j++)
        counter++;
    // usleep(1000000);
    printf("Thread %d finished\n", *(int *)i+1);
    return NULL;
}


int main()
{
    pthread_t    thread1[5];
    int i1[5];
    int i=0;

    while (i < 5)
    {
        i1[i] = i;
        pthread_create(&thread1[i],NULL,hello_thread, &i1[i]);
        i++;
    }

    for(int j = 0; j < 5; j++)
    {
        pthread_join(thread1[j],NULL);

    }
    printf("%lld \n", counter);
    return 0;
}