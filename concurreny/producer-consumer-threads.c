#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <assert.h>

#include "common_threads.h"

pthread_cond_t empty, full;
pthread_mutex_t m;


int buffer;
int count = 0; // initially, empty
int loops = 10;

void put(int value) {
    assert(count == 0);
    count = 1;
    buffer = value;
}

int get() {
    assert(count == 1);
    count = 0;
    return buffer;
}

void *producer(void *arg) {
    for (int i = 0; i < loops; i++) {
        pthread_mutex_lock(&m);                         // p1
        while (count == 1)                              // p2
            pthread_cond_wait(&empty, &m);               // p3
        put(i);                                         // p4
        pthread_cond_signal(&full);                     // p5
        pthread_mutex_unlock(&m);                       // p6
    }
}

void *consumer(void *arg) {
    for (int i = 0; i < loops; i++) {  
        pthread_mutex_lock(&m);                         // c1
        while (count == 0)                              // c2
            pthread_cond_wait(&full, &m);               // c3
        int tmp = get();                                // c4
        pthread_cond_signal(&empty);                     // c5
        pthread_mutex_unlock(&m);                       // c6
        printf("%d\n", tmp); 
    }
}