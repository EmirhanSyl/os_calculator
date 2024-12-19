#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// Function to send data to a specific child process
void send_to_child(int write_fd, int num1, int num2) {
    write(write_fd, &num1, sizeof(int));
    write(write_fd, &num2, sizeof(int));
}

// Function to read the result from a child process
int read_from_child(int read_fd) {
    int result;
    read(read_fd, &result, sizeof(int));
    return result;
}

// Function to create a subprocess
pid_t create_subprocess(const char *operation_program, int *pipe_to_child, int *pipe_from_child) {
    if (pipe(pipe_to_child) == -1 || pipe(pipe_from_child) == -1) {
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
        close(pipe_to_child[1]);  // Close write end of input pipe
        close(pipe_from_child[0]);  // Close read end of output pipe

        char read_fd_str[10], write_fd_str[10];
        snprintf(read_fd_str, sizeof(read_fd_str), "%d", pipe_to_child[0]);
        snprintf(write_fd_str, sizeof(write_fd_str), "%d", pipe_from_child[1]);

        execlp(operation_program, operation_program, read_fd_str, write_fd_str, NULL);
        perror("execlp");  // Only runs if exec fails
        exit(EXIT_FAILURE);
    }

    // Parent process
    close(pipe_to_child[0]);  // Close read end of input pipe
    close(pipe_from_child[1]);  // Close write end of output pipe
    return pid;
}

int main() {
    int pipe_to_add[2], pipe_from_add[2];
    int pipe_to_sub[2], pipe_from_sub[2];
    int pipe_to_mul[2], pipe_from_mul[2];
    int pipe_to_div[2], pipe_from_div[2];

    // Create subprocesses
    create_subprocess("./addition", pipe_to_add, pipe_from_add);
    create_subprocess("./subtraction", pipe_to_sub, pipe_from_sub);
    create_subprocess("./multiplication", pipe_to_mul, pipe_from_mul);
    create_subprocess("./division", pipe_to_div, pipe_from_div);

    while (1) {
        printf("\nChoose an operation:\n");
        printf("1- Addition\n");
        printf("2- Subtraction\n");
        printf("3- Division\n");
        printf("4- Multiplication\n");
        printf("5- Exit\n");
        printf("Enter your choice: ");

        int choice;
        scanf("%d", &choice);

        if (choice == 5) {
            printf("Exiting...\n");
            break;
        }

        if (choice < 1 || choice > 5) {
            printf("Invalid choice, please try again.\n");
            continue;
        }

        int num1, num2;
        printf("Enter two integers: ");
        scanf("%d %d", &num1, &num2);

        switch (choice) {
            case 1:  // Addition
                send_to_child(pipe_to_add[1], num1, num2);
                printf("Result: %d\n", read_from_child(pipe_from_add[0]));
                break;
            case 2:  // Subtraction
                send_to_child(pipe_to_sub[1], num1, num2);
                printf("Result: %d\n", read_from_child(pipe_from_sub[0]));
                break;
            case 3:  // Division
                if (num2 == 0) {
                    printf("Error: Division by zero is not allowed.\n");
                    continue;
                }
                send_to_child(pipe_to_div[1], num1, num2);
                printf("Result: %d\n", read_from_child(pipe_from_div[0]));
                break;
            case 4:  // Multiplication
                send_to_child(pipe_to_mul[1], num1, num2);
                printf("Result: %d\n", read_from_child(pipe_from_mul[0]));
                break;
            default:
                printf("Unknown error.\n");
        }
    }

    // Close all remaining pipes before exiting
    close(pipe_to_add[1]);
    close(pipe_from_add[0]);
    close(pipe_to_sub[1]);
    close(pipe_from_sub[0]);
    close(pipe_to_mul[1]);
    close(pipe_from_mul[0]);
    close(pipe_to_div[1]);
    close(pipe_from_div[0]);

    return 0;
}
