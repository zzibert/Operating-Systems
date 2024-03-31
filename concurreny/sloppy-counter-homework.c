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
    
    int global;
    pthread_mutex_t glock;
    int local[8];
    pthread_mutex_t llock[8];
    int threshold;
} counter_t;

void init(counter_t *c, int threshold) {
    c->threshold = threshold;
    c->global = 0;
    Pthread_mutex_init(&c->glock, NULL);
    for (int i = 0; i < 8; i++) {
        c->local[i] = 0;
        Pthread_mutex_init(&c->llock[i], NULL);
    }
}

void update(counter_t *c, int threadID, int amt) {
    int cpu = threadID % 8;
    Pthread_mutex_lock(&c->llock[cpu]);
    c->local[cpu] += amt;
    if (c->local[cpu] >= c->threshold) {
        Pthread_mutex_lock(&c->glock);
        c->global += c->local[cpu];
        Pthread_mutex_unlock(&c->glock);
        c->local[cpu] = 0;
    }
    Pthread_mutex_unlock(&c->llock[cpu]);
}

int get(counter_t *c) {
    Pthread_mutex_lock(&c->glock);
    int amt = c->global;
    Pthread_mutex_unlock(&c->glock);
    return amt;
}

counter_t *COUNTER;
int ITERATIONS = 100000;
int NUMBER_OF_THREADS = 8;

void* worker(void* arg) {
    for(int i = 0; i < ITERATIONS; i++) {
        pthread_t thread_id = pthread_self();
        uintptr_t thread_id_int = (uintptr_t) thread_id;
        int thread_id_casted = (int) thread_id_int;
        update(COUNTER, thread_id_casted, 1);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    COUNTER = malloc(sizeof(counter_t));
    init(COUNTER, 1024);

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