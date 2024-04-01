#include <stdio.h>
#include <stdlib.h>
#include "common_threads.h"

void *child(void *arg) {
    printf("child\n");
    /// how to indicate we are done?
    return NULL;
}

int main(int argc, char *argv[]) {
    printf("parent: begin\n");
    pthread_t c;
    Pthread_create(&c, NULL, child, NULL); // create child
    printf("parent: end\n");
    return 0;
}