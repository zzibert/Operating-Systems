#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int *p = (int *) malloc(sizeof(int));

    p = NULL;

    printf("this is the value of p: %d\n", *p);
}