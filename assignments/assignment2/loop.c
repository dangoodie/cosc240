#include <stdio.h>
#include <stdlib.h>

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
  for (int i = 0; i < VALUES_SIZE; i++) {
    total += calculate_value(values[i]);
  }
  printf("%d\n", total);
}
