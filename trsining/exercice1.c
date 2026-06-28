#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

long long counter = 0;
typedef struct threa
{
    pthread_mutex_t mutex1;
    pthread_t   thread;
    int tid_num;
}threa;


void* hello_thread(void* i)
{
    printf("thread %i is working",(threa *)i->tid_num)
    
}


int main()
{
    threa *tit1;
    threa *tit2;
    threa *tit3;

    tit1 = (threa*) malloc(sizeof(threa));
    if (!tit1)
    {
        return 0;
    }
    tit2 = (threa*) malloc(sizeof(threa));
    if (!tit2)
    {
        free(tit1);
        return 0;
    }
    tit3 = (threa*) malloc(sizeof(threa));
    if (!tit3)
    {
        free(tit1);
        free(tit2);
        return 0;
    }

    pthread_create(&(tit1->thread),NULL,hello_thread,tit1);
    pthread_create(&(tit2->thread),NULL,hello_thread,tit2);
    pthread_create(&(tit3->thread),NULL,hello_thread,tit3);

    pthread_join(tit1->thread, NULL);
    pthread_join(tit2->thread, NULL);
    pthread_join(tit3->thread, NULL);
    
    printf("%lld \n", counter);
    return 0;
}