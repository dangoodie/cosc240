#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// The values being used by this program
#define VALUE_MULTIPLIER 8
#define VALUE_ADDED 6
#define VALUES_SIZE 9
int values[] = {1, 8, 7, 8, 5, 1, 7, 9, 4};

// The value being calculated by this program
int total = 0;

pthread_mutex_t mutex;

// Perform a calculation on the given value
int calculate_value(int value) {
  return VALUE_MULTIPLIER * value + VALUE_ADDED;
}

// The function to be executed by each thread
void *thread_function(void *arg) {
  int index = *(int *)arg; // Convert the void pointer to an integer
  int value = calculate_value(values[index]);

  int status;
  status = pthread_mutex_lock(&mutex);
  if (status != 0) {
    perror("mutex_lock");
    exit(2);
  }
  total += value;
  status = pthread_mutex_unlock(&mutex);
  if (status != 0) {
    perror("pthread_mutex_unlock() error");
    exit(2);
  }

  return NULL;
}

// Program entry point
// Command line arguments are ignored
int main(int argc, char *argv[]) {
  pthread_t threads[VALUES_SIZE];
  int indexes[VALUES_SIZE];

  int status;

  // Initialize the mutex
  status = pthread_mutex_init(&mutex, NULL);
  if (status != 0) {
    perror("mutex_lock");
    exit(1);
  }

  // Create all the threads and get them to execute thread_function
  for (int i = 0; i < VALUES_SIZE; i++) {
    indexes[i] = i;
    status = pthread_create(&threads[i], NULL, thread_function, &indexes[i]);
    if (status != 0) {
      perror("Error creating child thread - exiting");
      exit(-1);
    }
  }

  // Wait for all the threads to complete
  for (int i = 0; i < VALUES_SIZE; i++) {
    status = pthread_join(threads[i], NULL);
    if (status != 0) {
      perror("Error joining child thread - exiting");
      exit(-2);
    }
  }

  // Clean up the mutex
  pthread_mutex_destroy(&mutex);

  // Output the result
  printf("%d\n", total);
}
