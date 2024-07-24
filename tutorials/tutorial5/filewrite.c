#include <stdio.h>

void remove_newline(char string[], int length) {
    for (int i = 0; i < length; i++) {
        if (string[i] == '\n') {
            string[i] = '\0';
            return;
        }
    }
}

int main() {
    int value = 100;
    int read_value;
    FILE *fp = fopen("readme.txt", "w");
    
    if (fp) {
        fprintf(fp, "Writing some output to the file\n");
        fprintf(fp, "We can do pretty much anything printf could do:\n%d\n", value);
        fclose(fp);
    }
    
    fp = fopen("readme.txt", "r");
    if (fp) {
        char line[1024]; // support lines up to 1024 characters only
        if (fgets(line, 1024, fp) != NULL) {
            remove_newline(line, 1024);
            printf("First line: %s\n", line);
        }
        if (fgets(line, 1024, fp) != NULL) {
            remove_newline(line, 1024);
            printf("Second line: %s\n", line);
        }
        if (fscanf(fp, "%d", &read_value) != EOF) {
            printf("Read value: %d\n", read_value);
        }
        fclose(fp);
    }

    return 0;
}
