#include <stdio.h>

int main() {
    int a = 3;
    int b = 2;
    
    float result1 = a / b;
    float result2 = a / (float) b;
    
    printf("Integer division: %f\n", result1);
    printf("Floating point division: %f\n", result2);
}
