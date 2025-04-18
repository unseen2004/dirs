#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t signal_count = 0;

void handle_signal(int sig) {
    signal_count++;
    printf("Odebrano sygnał %d, liczba odebranych sygnałów: %d\n", sig, signal_count);
    sleep(5); 
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    printf("PID procesu: %d\n", getpid());

    while (1) {
        pause();
    }

    return 0;
}
