#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <result>\n", argv[0]);
        return 1;
    }

    const char *result = argv[1];
    FILE *file = fopen("results.txt", "a");
    if (!file) {
        perror("fopen failed");
        return 1;
    }

    fprintf(file, "Result: %s\n", result);
    fclose(file);
    return 0;
}
