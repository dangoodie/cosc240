#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// The values being used by this program
#define VALUE_MULTIPLIER 8
#define VALUE_ADDED 6
#define VALUES_SIZE 9
int values[] = {1, 8, 7, 8, 5, 1, 7, 9, 4};

// The value being calculated by this program
int total = 0;

// Perform a calculation on the given value
int calculate_value(int value) {
  return VALUE_MULTIPLIER * value + VALUE_ADDED;
}

// Program entry point
// Command line arguments are ignored
int main(int argc, char *argv[]) {
  pid_t children[VALUES_SIZE]; // Array to store the child processes

  for (int i = 0; i < VALUES_SIZE; i++) {
    children[i] = fork();
    if (children[i] == -1) {
      // Handle errors
      perror("fork");
      exit(EXIT_FAILURE);
    } else if (children[i] == 0) {
      // Child process
      int result = calculate_value(values[i]);
      exit(result); // Exit with the result
    }
  }

  // Parent process
  for (int i = 0; i < VALUES_SIZE; i++) {
    int status;
    waitpid(children[i], &status, 0);
    if (WIFEXITED(status)) {
      total += WEXITSTATUS(status);
    } else {
      // Handle error
      fprintf(stderr, "Child process %d did not terminate normally!\n",
              children[i]);
    }
  }

  printf("%d\n", total);
}
