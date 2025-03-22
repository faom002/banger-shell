#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_BLUE  "\033[1;34m"
#define COLOR_RED   "\033[1;31m"
#define COLOR_YELLOW "\033[1;33m"
#define STATUS_BAR_COLOR "\033[48;5;238m\033[38;5;15m"
#define STATUS_BAR_RESET "\033[0m"
#define MOVE_CURSOR_BOTTOM "\033[999;0H"
#define MOVE_CURSOR_UP "\033[%dA"

#define MAX_INPUT 1024
#define MAX_ARGS 40

char *is_git_initialized() {
    struct stat statbuf;
    if (stat(".git", &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
        return "🤖";
    }
    return "❌";
}

int get_terminal_width() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        return w.ws_col;
    }
    return 80;  
}

int main () {
    char input[MAX_INPUT];
    int status;
    char cwd[1024];

    while (1) {
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf(COLOR_RED "%s\n" COLOR_RESET, cwd);
            printf(COLOR_YELLOW "Banger-shell⟁ " COLOR_RESET);
            printf(MOVE_CURSOR_BOTTOM);
            printf("\n\n");

            int terminal_width = get_terminal_width();

            printf(STATUS_BAR_COLOR " ⟁ STATUS: Ready | GIT INITIALIZED: %s ", is_git_initialized());

            int remaining_width = terminal_width - 34 - strlen(is_git_initialized()) - 18; 
            for (int i = 0; i < remaining_width; i++) {
                printf(" ");
            }
            printf(STATUS_BAR_RESET "\n");

            printf("\033[3A");
            printf("\033[14C");
        } else {
            perror("getcwd");
            continue;
        }

        fflush(stdout);

        if (fgets(input, MAX_INPUT, stdin)) {
            input[strcspn(input, "\n")] = 0;  

            char *args[MAX_ARGS];
            int i = 0;
            char *token = strtok(input, " ");

            while (token != NULL && i < MAX_ARGS - 1) {
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
                    fprintf(stderr, COLOR_YELLOW "cd: missing argument\n" COLOR_RESET);
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

