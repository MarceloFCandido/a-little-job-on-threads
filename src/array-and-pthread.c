#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define ARRAY_SIZE 1024
#define NUM_THREADS 4

typedef struct kit_t {
    int thread_id;
    int *array;
} KIT_t;

void *array_build(void *kit) {
    // Casting the kit
    KIT_t *kit_casted = (KIT_t *) kit;
    int thread_id = kit_casted->thread_id;
    int *array = kit_casted->array;

    // determining how many points this thead should evaluate
    int num_positions = ARRAY_SIZE / NUM_THREADS;

    for (int i = thread_id * num_positions;
                                    i < (thread_id + 1) * num_positions; i++) {
        array[i] = rand() % ARRAY_SIZE;
    }

    pthread_exit(NULL);
}

void *array_greatest(void *kit) {
    // Casting the kit
    KIT_t *kit_casted = (KIT_t *) kit;

    int thread_id = kit_casted->thread_id;
    int *array = kit_casted->array;
    int *result;

    int num_positions = ARRAY_SIZE / NUM_THREADS;

    result = malloc(sizeof(int));
    *result = 0;

    for (int i = thread_id * num_positions;
                                    i < (thread_id + 1) * num_positions; i++) {
        // comparing if the ith element is greater than the last result
        if(array[i] > *result) {
            *result = array[i];
        }
    }

    // returning result for the caller and exiting the thread
    pthread_exit(result);
}

void array_print(int *array) {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("array[%10d]: %10d\n", i, array[i]);
    }
}

int main(int argc, char const *argv[]) {

    srand(time(0));

    void *thread_return;
    int result = 0;

    // Creating a int array of 4 MB
    int *array = (int *) malloc(ARRAY_SIZE * sizeof(int));

    // Creating array of thread type and attributes
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    KIT_t kit[NUM_THREADS];

    // Initializing kits
    for (int i = 0; i < NUM_THREADS; i++) {
        kit[i].array = array;
        kit[i].thread_id = i;
    }

    // Creating threads for fill the array
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], &attr, array_build, (void *) &kit[i])) {
            // error
            printf("Error creating thread %d", i);
            exit(-1);
        }
    }

    // Joining threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL)) {
            // error
            printf("Error joining thread %d", i);
            exit(-1);
        }
    }
    //Creating threads for searching the greatest element on the array
        for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], &attr, array_greatest, (void *) &kit[i])) {
            // error
            printf("Error creating thread %d", i);
            exit(-1);
        }
     }

    // Joining threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], &thread_return)) {
            // error
            printf("Error joining thread %d", i);
            exit(-1);
        }
        if (result < *(int *) thread_return){
            result = *(int *) thread_return;
        }
     }

    array_print(array);

    printf("The greatest element in the array is %d.\n", result);

    return 0;
}
