#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to save the result using the saver program
void save_result(int num1, int num2, int result) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process: Execute the saver program
        char res_str[100];
        snprintf(res_str, sizeof(res_str), "Result: %d + %d = %d", num1, num2, result);
        execl("./saver", "saver", res_str, NULL);
        perror("execl saver failed");
        exit(1);
    } else if (pid > 0) {
        // Parent process: Wait for the saver process to complete
        wait(NULL);
    } else {
        perror("fork failed for saver");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <read_fd> <write_fd>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int read_fd = atoi(argv[1]);
    int write_fd = atoi(argv[2]);

    while (1) {  // Keep running to process multiple requests
        int num1, num2;
        read(read_fd, &num1, sizeof(int));
        read(read_fd, &num2, sizeof(int));

        int result = num1 + num2;
        save_result(num1, num2, result);

        write(write_fd, &result, sizeof(int));
    }

    return 0;
}
