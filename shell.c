#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024
#define TOKEN_DELIMITERS " \t\r\n\a"

// Function to read a line of input
char *read_line() {
    char *line = NULL;
    ssize_t buffer_size = 0;
    getline(&line, &buffer_size, stdin);
    return line;
}

// Function to parse input into tokens
char **parse_line(char *line) {
    int buffer_size = BUFFER_SIZE;
    int position = 0;
    char **tokens = malloc(buffer_size * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, "Allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, TOKEN_DELIMITERS);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= buffer_size) {
            buffer_size += BUFFER_SIZE;
            tokens = realloc(tokens, buffer_size * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TOKEN_DELIMITERS);
    }
    tokens[position] = NULL;
    return tokens;
}

// Function to execute commands
void execute_command(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("Execution error");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Forking error
        perror("Forking error");
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

// Main function
int main() {
    char *line;
    char **args;

    printf("Custom Shell: Type 'exit' to quit\n");

    while (1) {
        printf("> ");
        line = read_line();
        args = parse_line(line);
        execute_command(args);

        free(line);
        free(args);
    }

    return EXIT_SUCCESS;
}
