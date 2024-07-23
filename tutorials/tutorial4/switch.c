#include <stdio.h>

int main() {

    int x = 0;
    
    switch (x) {
    case 0:
        printf("Zero!\n");
        break;
    case 1:
        printf("One!\n");
        break;
    default:
        printf("Some other value!\n");
        break;
    }

    return 0;
}
