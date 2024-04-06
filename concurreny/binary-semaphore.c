#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"
#include "common_threads.h"

sem_t m;
sem_init(&m, 0, X);

sem_wait(&m);
// critical section here
sem_post(&m);