#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int sig) {
    printf("Odebrano sygnał: %d\n", sig);
}

int main() {
    for (int i = 1; i < NSIG; i++) {
        if (i == SIGKILL || i == SIGSTOP) {
            continue; 
        }
        signal(i, handle_signal);
    }

    printf("PID procesu: %d\n", getpid());

    while (1) {
        pause(); 
    }

    return 0;
}
