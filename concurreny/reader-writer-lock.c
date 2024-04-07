#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "common.h"
#include "common_threads.h"

#ifdef linux
#include <semaphore.h>
#elif __APPLE__
#include "zemaphore.h"
#endif

typedef struct {
    sem_t lock; // binary semaphore (basic lock)
    sem_t writelock; // used to allow ONE writer or MANY readers
    int readers; // count of readers reading in critical section
} rwlock_t;

void rwlock_init(rwlock_t *rw) {
    rw->readers = 0;
    Sem_init(&rw->lock, 1);
    Sem_init(&rw->writelock, 1);
}

void rwlock_acquire_readlock(rwlock_t *rw) {
    Sem_wait(&rw->lock);
    rw->readers++;
    if (rw->readers == 1)
        Sem_wait(&rw->writelock); // first reader acquires writelock
    Sem_post(&rw->lock);
}

void rwlock_release_readlock(rwlock_t *rw) {
    Sem_wait(&rw->lock);
    rw->readers--;
    if (rw->readers == 0)
        Sem_post(&rw->writelock); // last reader releases writelock
    Sem_post(&rw->lock);
}

void rwlock_acquire_writelock(rwlock_t *rw) {
    Sem_wait(&rw->writelock);
}

void rwlock_release_writelock(rwlock_t *rw) {
    Sem_post(&rw->writelock);
}