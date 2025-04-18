#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

void change_stdout(int pid, const char *new_output) {
    // Otwieramy nowy plik dla wyjścia
    int new_fd = open(new_output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (new_fd == -1) {
        perror("open");
        exit(1);
    }

    // Śledzimy proces za pomocą ptrace
    if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) == -1) {
        perror("ptrace_attach");
        close(new_fd);
        exit(1);
    }

    // Czekamy na zatrzymanie procesu
    waitpid(pid, NULL, 0);

    // Podmieniamy deskryptor wyjścia (1) na deskryptor nowego pliku
    if (dup2(new_fd, 1) == -1) {
        perror("dup2");
        ptrace(PTRACE_DETACH, pid, NULL, NULL);
        close(new_fd);
        exit(1);
    }

    // Zamykamy deskryptor pliku, nie jest już potrzebny
    close(new_fd);

    // Kontynuujemy działanie procesu
    if (ptrace(PTRACE_DETACH, pid, NULL, NULL) == -1) {
        perror("ptrace_detach");
        exit(1);
    }

    printf("Wyjście procesu %d zostało przekierowane do %s\n", pid, new_output);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Użycie: %s <PID> <plik_wyjściowy>\n", argv[0]);
        exit(1);
    }

    int pid = atoi(argv[1]);
    const char *new_output = argv[2];

    change_stdout(pid, new_output);

    return 0;
}

