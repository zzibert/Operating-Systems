#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <assert.h>

#include "common_threads.h"

int bytesLeft = MAX_HEAP_SIZE;



pthread_cond_t c;
pthread_mutex_t m;

void *allocate(int size) {
    Pthread_mutex_lock(&m);
    while (bytesLeft < size) Pthread_cond_wait(&c, &m);
    void *ptr = "...";
    bytesLeft -= size;
    Pthread_mutex_unlock(&m);
    return ptr;
}

void free(void *ptr, int size) {
    Pthread_mutex_lock(&m);
    bytesLeft += size;
    Pthread_cond_signal(&c); // whom to signal?
    Pthread_mutex_unlock(&m);
}