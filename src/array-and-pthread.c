#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 1024 * 1024

int main(int argc, char const *argv[]) {

    // Creating a int array of 4 MB
    int *array = (int *) malloc(ARRAY_SIZE * sizeof(int));

    

    return 0;
}
