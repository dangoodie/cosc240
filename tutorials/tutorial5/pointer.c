#include <stdlib.h>
#include <stdio.h>

struct example {
    int a;
    double b;
};

struct example *new_example(int a) {
    struct example *ptr = malloc(sizeof(*ptr));
    if (ptr) {
        ptr->a = a;
        ptr->b = 0;
    }
    return ptr;
}

int main() {

    struct example *test = new_example(5);

    if (test) {
        test->b = 0.5;
    
        printf("%d, %f\n", test->a, test->b);
        
        free(test);
        test = 0;
    }

    return 0;
}

