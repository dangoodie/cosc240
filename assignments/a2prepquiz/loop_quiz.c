#include <stdio.h>
#include <stdlib.h>

// The values being used by this program
#define VALUES_SIZE 5
int values[] = {5, 7, 2, 4, 9};

// The value being calculated by this program
int total = 0;

int calculate_value(int value) {
    return value;
}

// Program entry point
// Command line arguments are ignored
int main(int argc, char *argv[]) {
    for (int i = 0; i < VALUES_SIZE; i++) {
        total += calculate_value(values[i]);
    }
    printf("%d\n", total);
}
