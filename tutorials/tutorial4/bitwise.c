#include <stdio.h>

int main() {
    unsigned int a = 0xF; // Using 0x allows us to enter hexadecimal values - this is the value with the four right-most bits set to 1
    unsigned int b = 0xFF; // The eight right-most bits are set to 1
    
    printf("a: %u\n", a); // %u is an unsigned int
    printf("~a: %u\n", ~a);
    printf("a & b: %u\n", a & b);
    printf("a | b: %u\n", a | b);
    printf("a ^ b: %u\n", a ^ b);
    printf("a << 2: %u\n", a << 2);
    printf("a >> 2: %u\n", a >> 2);

    return 0;
}

