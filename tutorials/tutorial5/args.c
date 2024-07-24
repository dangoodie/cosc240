#include <stdio.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: %s <arg> <arg>...\n", argv[0]);
        printf("Where: <arg> is any number of command line arguments\n");
    } else {
        printf("%s called with the following arguments:\n", argv[0]);
        for (int i = 1; i < argc; i++) {
            printf("\t%s\n", argv[i]);
        }
    }

    return 0;
}

