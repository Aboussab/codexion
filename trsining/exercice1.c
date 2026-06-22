#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


void* hello_thread(void* arg) {
    unsigned long thread_id = (unsigned long)(pthread_self());
    printf("Hello from %s with id : %ld!\n", (char *)arg, thread_id);
    sleep(1);
    printf("%s %ld is done.\n", (char*)arg, thread_id);
    return NULL;
}

int main()
{
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_t thread4;
    pthread_t thread5;


    printf("Hello from thread %ld!\n", (unsigned long)pthread_self());


    if (pthread_create(&thread1, NULL, hello_thread, "thread1") != 0)
        printf("ther is an error while creating threads.");
    if (pthread_create(&thread2, NULL, hello_thread, "thread2") != 0)
        printf("ther is an error while creating threads.");
    if (pthread_create(&thread3, NULL, hello_thread, "thread3") != 0)
        printf("ther is an error while creating threads.");
    if (pthread_create(&thread4, NULL, hello_thread, "thread4") != 0)
        printf("ther is an error while creating threads.");
    if (pthread_create(&thread5, NULL, hello_thread, "thread5") != 0)
        printf("ther is an error while creating threads.");
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    pthread_join(thread3,NULL);
    pthread_join(thread4,NULL);
    pthread_join(thread5,NULL);
}


// int main() {
    // pthread_t tid1, tid2;
    // int id1 = 1, id2 = 2;
    // 
    // printf("Main: Creating threads...\n");
    // 
    // Create two threads
    // if (pthread_create(&tid1, NULL, hello_thread, &id1) != 0) {
        // perror("pthread_create");
        // exit(1);
    // }
    // 
    // if (pthread_create(&tid2, NULL, hello_thread, &id2) != 0) {
        // perror("pthread_create");
        // exit(1);
    // }
    // 
    // printf("Main: Waiting for threads to finish...\n");
    // 
    // Wait for threads to complete
    // pthread_join(tid1, NULL);
    // pthread_join(tid2, NULL);
    // 
    // printf("Main: All threads completed. Exiting.\n");
    // return 0;
// }