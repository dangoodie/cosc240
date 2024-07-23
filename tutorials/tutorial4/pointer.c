#include <stdio.h>

void print_variable(char name, int *pointer) {
    // print out the name of the variable, the memory location, and the value stored in that memory location
    // we only pass the pointer because we can dereference it to get the actual value!
    printf("%c is in memory location %p and has value %d\n", name, pointer, *pointer);  // %p is the format string for a pointer
}

int main() {
    int x = 3;
    int y;
    
    print_variable('x', &x);
    print_variable('y', &y);  // Note that, since y is uninitialised, it could have any value!
    
    printf("Enter a value for y: ");
    scanf("%d", &y);
    print_variable('x', &x);
    print_variable('y', &y);
    
    int *p = &x;  // p now points to the same memory location that stores x
    *p = y;  // we're storing the value of y in the memory location pointed to by p
    print_variable('x', &x);
    print_variable('y', &y);
    
    p = &y;  // p now points to the same memory location that stores y
    *p = 3;  // we're storing 3 in the memory location pointed to by p
    print_variable('x', &x);
    print_variable('y', &y);
    
    return 0;
}

