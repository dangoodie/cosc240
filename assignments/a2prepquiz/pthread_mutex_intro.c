#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// The values to use for example purposes
#define NUM_THREADS 2
int VALUE1 = 2;
int VALUE2 = 3;

// The value being calculated by this program
int total = 0;
// The mutex to use to protect access to total
pthread_mutex_t total_mutex;

// A function to add a value to total
// value is a pointer to the address of the integer to add
void *calculate_value(void *value) {
        // Wait for exclusive access to the total_mutex
        int status;
        status = pthread_mutex_lock(&total_mutex);
        if (status != 0) {
                perror("mutex_lock");
                exit(2);
        }

        // Perform the calculation
        int val = *((int *)value); // Convert parameter to an integer
        total += val;

        // Give up exclusive access to the total_mutex
        status = pthread_mutex_unlock(&total_mutex);
        if (status != 0) {
                perror("pthread_mutex_unlock() error");
                exit(2);
        }
}

// Program entry point
// Command line arguments are ignored
int main(int argc, char *argv[]) {
        int status;
        // Initialise total_mutex
        status = pthread_mutex_init(&total_mutex, NULL);
        if (status != 0) {
                perror("mutex_lock");
                exit(1);
        }
        // The threads used in this program
        pthread_t threads[NUM_THREADS];

        // A loop to create all the threads
        for (int i = 0; i < NUM_THREADS; i++) {
                int *value = (i == 0) ? &VALUE1 : &VALUE2;
                // Create the thread and get it to execute calculate_value
                status = pthread_create(&threads[i], NULL, calculate_value, (void *)(value));
                // If there's an error creating a child thread, just exit
                if (status != 0) {
                        perror("Error creating child thread - exiting");
                        exit(-1);
                }
        }

        // A loop to wait for all threads to finish processing
        for (int i = 0; i < NUM_THREADS; i++) {
                // This process only continues when threads[i] completes
                int status = pthread_join(threads[i], NULL);
                // If there's an error waiting for a child thread, just exit
                if (status != 0) {
                        perror("Error joining child thread - exiting");
                        exit(-2);
                }
        }

        // We no longer require the mutex, so it can be destroyed
        if (pthread_mutex_destroy(&total_mutex) != 0) {
                perror("pthread_mutex_destroy() error");
                exit(2);
        }

        // Get exclusive access to the value variable and print it out
        printf("%d\n", total);
}

