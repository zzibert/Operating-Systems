#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <assert.h>

#include "common_threads.h"

pthread_cond_t cond;
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
        pthread_mutex_lock(&m);
        if (count == 1) pthread_cond_wait(&cond, &m);
        put(i);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&m);
    }
}

void *consumer(void *arg) {
    for (int i = 0; i < loops; i++) {
        pthread_mutex_lock(&m);
        if (count == 0) pthread_cond_wait(&cond, &m);
        int tmp = get();
        printf("%d\n", tmp);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&m);
    }
}