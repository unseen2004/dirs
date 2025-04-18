#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp = popen("tput colors", "r");
    int max_colors = 8;  

    if (fp != NULL) {
        fscanf(fp, "%d", &max_colors);
        pclose(fp);
    }

    for (int i = 0; i < max_colors; i++) {
        printf("\033[38;5;%dmHello, World!\n", i);
    }

    printf("\033[0m");

    return 0;
}



