#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define MAX_LINE 1024
#define MAX_ARGS 100

// Obsługa procesów zombie
void handle_sigchld(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

// Obsługa Ctrl+C
void handle_sigint(int sig) {
    printf("\nPrzerwano działanie programu.\n");
}

// Funkcja cd (komenda wbudowana)
int lsh_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "lsh: oczekiwano argumentu dla \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("lsh");
        }
    }
    return 1;
}

// Funkcja exit (komenda wbudowana)
int lsh_exit(char **args) {
    return 0;
}

// Parsowanie linii wejścia na argumenty
char **lsh_parse_line(char *line) {
    int bufsize = MAX_ARGS, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, "lsh: błąd alokacji pamięci\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\r\n");
    while (token != NULL) {
        tokens[position++] = token;

        if (position >= bufsize) {
            bufsize += MAX_ARGS;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "lsh: błąd alokacji pamięci\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " \t\r\n");
    }
    tokens[position] = NULL;
    return tokens;
}

// Wykonanie polecenia z potokami i przekierowaniami
int lsh_execute(char **args) {
    if (args[0] == NULL) {
        // Pusta linia
        return 1;
    }

    // Komendy wbudowane
    if (strcmp(args[0], "cd") == 0) {
        return lsh_cd(args);
    }
    if (strcmp(args[0], "exit") == 0) {
        return lsh_exit(args);
    }

    // Obsługa procesów
    pid_t pid, wpid;
    int status;

    // Przetwarzanie potoków
    int pipefds[2];
    int has_pipe = 0;
    char *pipe_command = NULL;

    // Szukanie potoku
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            has_pipe = 1;
            args[i] = NULL;
            pipe_command = args[i + 1];
            break;
        }
    }

    if (has_pipe) {
        // Utwórz potok
        if (pipe(pipefds) == -1) {
            perror("pipe");
            return 1;
        }
    }

    pid = fork();
    if (pid == 0) {
        // Proces potomny

        // Obsługa przekierowania wejścia (<)
        for (int i = 0; args[i] != NULL; i++) {
            if (strcmp(args[i], "<") == 0) {
                int fd = open(args[i + 1], O_RDONLY);
                if (fd < 0) {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
                args[i] = NULL;
                break;
            }
        }

        // Obsługa przekierowania wyjścia (>)
        for (int i = 0; args[i] != NULL; i++) {
            if (strcmp(args[i], ">") == 0) {
                int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
                args[i] = NULL;
                break;
            }
        }

        // Obsługa przekierowania wyjścia błędów (2>)
        for (int i = 0; args[i] != NULL; i++) {
            if (strcmp(args[i], "2>") == 0) {
                int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    perror("open");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDERR_FILENO);
                close(fd);
                args[i] = NULL;
                break;
            }
        }

        if (has_pipe) {
            close(pipefds[0]);
            dup2(pipefds[1], STDOUT_FILENO);
            close(pipefds[1]);
        }

        if (execvp(args[0], args) == -1) {
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("lsh");
    } else {
        if (has_pipe) {
            // Druga część potoku
            pid_t pipe_pid = fork();
            if (pipe_pid == 0) {
                close(pipefds[1]);
                dup2(pipefds[0], STDIN_FILENO);
                close(pipefds[0]);

                char *pipe_args[] = {pipe_command, NULL};
                if (execvp(pipe_command, pipe_args) == -1) {
                    perror("lsh");
                }
                exit(EXIT_FAILURE);
            }
            close(pipefds[0]);
            close(pipefds[1]);
            waitpid(pipe_pid, NULL, 0);
        }
        // Czekaj na zakończenie procesu
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

// Główna pętla powłoki
void lsh_loop() {
    char *line = NULL;
    size_t bufsize = 0;
    char **args;
    int status = 1;

    // Ustawienie obsługi SIGCHLD i SIGINT
    struct sigaction sa_chld, sa_int;
    sa_chld.sa_handler = handle_sigchld;
    sa_chld.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa_chld, NULL);

    sa_int.sa_handler = handle_sigint;
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);

    do {
        printf("lsh> ");
        if (getline(&line, &bufsize, stdin) == -1) {
            printf("\n");
            break;
        }
        args = lsh_parse_line(line);
        status = lsh_execute(args);

        free(args);
    } while (status);

    free(line);
}

// Funkcja main
int main(int argc, char **argv) {
    lsh_loop();
    return 0;
}

