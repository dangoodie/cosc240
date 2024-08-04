#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// A value to use for example purposes
#define VALUE 2

// The values we want the thread to use
typedef struct thread_values {
    int input;
    int output;
} thread_values;

// A function to add a value to total
// value is a pointer to the address of the thread_values
void *calculate_value(void *value) {
        // Convert the void pointer into the thread_values it points to
        thread_values *vals = (thread_values *)value;
        // Perform the calculation
        vals->output = vals->input;
}

// Program entry point
// Command line arguments are ignored
int main(int argc, char *argv[]) {
        // The child thread used in this program
        pthread_t thread;

        // The value for the thread to use
        thread_values calculated;
        calculated.input = VALUE;
        
        // Create the thread and get it to execute calculate_value
        int status = 0;
        status = pthread_create(&thread, NULL, calculate_value, (void *)&(calculated));
        // If there's an error creating the child thread, just exit
        if (status != 0) {
                perror("Error creating child thread - exiting");
                exit(-1);
        }

        // This process only continues when the child thread completes
        status = pthread_join(thread, NULL);
        // If there's an error waiting for the child thread, just exit
        if (status != 0) {
                perror("Error joining child thread - exiting");
                exit(-2);
        }

        // The child is guaranteed to have completed, so we can access its output value
        printf("%d\n", calculated.output);
}
