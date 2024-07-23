#include <stdlib.h>
#include <stdio.h>

int main() {
    int *x = malloc(sizeof(*x));  // Get some new memory large enough to store the type of x
    
    if (!x) {
        printf("malloc returned 0! That means there was no memory available.\n");
    } else {
    
        printf("x points to memory location %p, which has value %d\n", x, *x);
        *x = 7;
        printf("x points to memory location %p, which has value %d\n", x, *x);
        free(x);
    }
    
    x = 0;
    printf("x points to memory location %p, which has value %d\n", x, *x);
    
    return 0;
}

