#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <assert.h>

#include "common_threads.h"

double Time_GetSeconds() {
    struct timeval t;
    int rc = gettimeofday(&t, NULL);
    assert(rc == 0);
    return (double) ((double)t.tv_sec + (double)t.tv_usec / 1e6);
}

typedef struct {
    int value;
    pthread_mutex_t lock;
} counter_t;

void init(counter_t *c) {
    c->value = 0;
    Pthread_mutex_init(&c->lock, NULL);
}

void increment(counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    c->value++;
    Pthread_mutex_unlock(&c->lock);
}

void decrement(counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    c->value--;
    Pthread_mutex_unlock(&c->lock);
}

int get(counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    int rc = c->value;
    Pthread_mutex_unlock(&c->lock);
    return rc;
}

counter_t *COUNTER;
int ITERATIONS = 100000;
int NUMBER_OF_THREADS = 32;

void* worker(void* arg) {
    for(int i = 0; i < ITERATIONS; i++) {
        increment(COUNTER);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    COUNTER = malloc(sizeof(counter_t));
    init(COUNTER);

    pthread_t thread_list[NUMBER_OF_THREADS];
    double start, end;

    start = Time_GetSeconds();

    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        Pthread_create(&thread_list[i], NULL, worker, (void *) (long long) 0);
    }

    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        Pthread_join(thread_list[i], NULL);
    }

    end = Time_GetSeconds();

    double time = end - start;

    
    printf("counter value is: %d, time is %f ", get(COUNTER), time);
    return 0;
}