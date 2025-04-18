#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    if (setuid(0) != 0) {
        perror("setuid failed");
        exit(1);
    }

    execlp("/bin/bash", "bash", (char *)NULL);

    perror("execlp failed");
    exit(1);
}

