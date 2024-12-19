#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int pipe1[2];  // Pipe for sending data to child
    int pipe2[2];  // Pipe for receiving data from child

    // Create pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        close(pipe1[1]);  // Close unused write end of pipe1
        close(pipe2[0]);  // Close unused read end of pipe2

        // Pass file descriptors as arguments
        char read_fd_str[10], write_fd_str[10];
        snprintf(read_fd_str, sizeof(read_fd_str), "%d", pipe1[0]);  // Read end of pipe1
        snprintf(write_fd_str, sizeof(write_fd_str), "%d", pipe2[1]); // Write end of pipe2

        execlp("./addition", "./addition", read_fd_str, write_fd_str, NULL);
        perror("execlp");  // Only runs if exec fails
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        close(pipe1[0]);  // Close unused read end of pipe1
        close(pipe2[1]);  // Close unused write end of pipe2

        // Send two numbers to the child process
        int num1 = 5, num2 = 7;
        write(pipe1[1], &num1, sizeof(int));
        write(pipe1[1], &num2, sizeof(int));
        close(pipe1[1]);  // Close write end after sending

        // Read the multiplication result from the child
        int result;
        read(pipe2[0], &result, sizeof(int));
        printf("Parent received: %d * %d = %d\n", num1, num2, result);

        close(pipe2[0]);  // Close read end
        wait(NULL);       // Wait for child to finish
    }

    return 0;
}
