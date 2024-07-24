#include <stdio.h>

int main() {
    FILE *fp;
    
    fp = fopen("readme.txt", "r");
    if (fp) {
        printf("Opened readme.txt!\n");
    } else {
        printf("Failed to open readme.txt!\n");
    }

    if (fp) {
        fclose(fp);
        printf("Closed readme.txt!\n");
    }
    return 0;
}
