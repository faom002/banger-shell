#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_INPUT 1024
#define MAX_ARGS 40

int main () {
    char input[MAX_INPUT];
    int status;

    while(1) {
        printf("Banger-shell⟁ ");
        fflush(stdout);

        if (fgets(input, MAX_INPUT, stdin)) {
            input[strcspn(input, "\n")] = 0; 

            char *args[MAX_ARGS];
            int i = 0;
            char *token = strtok(input, " ");

            while(token != NULL && i < MAX_ARGS - 1) {
                args[i] = token;
                token = strtok(NULL, " ");
                i++;
            }

            args[i] = NULL;

            if (strcmp(args[0], "exit") == 0) {
                break;
            }

            if (strcmp(args[0], "cd") == 0) {
                if (args[1] != NULL) {
                    if (chdir(args[1]) != 0) {
                        perror("cd");
                    }
                } else {
                    fprintf(stderr, "cd: missing argument\n");
                }
                continue;  
            }

            
            int rc = fork();
            if (rc == -1) {
                perror("fork");
                exit(EXIT_FAILURE);
            } else if (rc == 0) {
                execvp(args[0], args);
                perror("execvp");  
                exit(0);
            } else {
                waitpid(rc, &status, 0);
                if (WIFEXITED(status)) {
                    printf("exited normally\n");
                }
                if (WIFSIGNALED(status)) {
                    printf("exited with signal: %d\n", WTERMSIG(status));
                }
            }
        }
    }

    return 0;
}

