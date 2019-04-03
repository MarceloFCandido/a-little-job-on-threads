#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define ARRAY_SIZE 1024 * 1024
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

    int num_positions = ARRAY_SIZE / NUM_THREADS;

    for (int i = thread_id * num_positions;
                                    i < (thread_id + 1) * num_positions; i++)
        array[i] = rand() % ARRAY_SIZE;

    pthread_exit(NULL);
}

void *array_greatest(void *kit) {
    KIT_t *kit_casted = (KIT_t *) kit;
}

int main(int argc, char const *argv[]) {

    srand(time(0));

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

    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%05d\n", array[i]);
    }

    // Creating threads for searching the greatest element on the array
    // for (int i = 0; i < NUM_THREADS; i++) {
    //     if (pthread_create(&threads[i], &attr, array_greatest, (void *) kit)) {
    //         // error
    //         printf("Error creating thread %d", i);
    //         exit(-1);
    //     }
    // }
    //
    // // Joining threads
    // for (int i = 0; i < NUM_THREADS; i++) {
    //     if (pthread_join(threads[i], NULL)) {
    //         // error
    //         printf("Error joining thread %d", i);
    //         exit(-1);
    //     }
    // }



    return 0;
}
