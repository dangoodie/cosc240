#include <stdio.h>
#include <stdlib.h>

// The values being used by this program
#define VALUE_MULTIPLIER 2
#define VALUE_ADDED 1
#define VALUES_SIZE 5
int values[] = {5, 7, 2, 4, 9};

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
