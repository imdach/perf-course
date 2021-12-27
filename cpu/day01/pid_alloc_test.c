#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int max = 0;
    int min = 32768;
    pid_t pid;
    int loop = 0;
    while (1) {
        pid = fork();
        if (pid < 0) {
            printf("fork error");
            break;
        } else if (pid == 0) {
            exit(0);
        } else {
            if (pid > max) max = pid;
            if (pid < min) min = pid;

            int exit_status;
            if (waitpid(pid, &exit_status, 0) < 0) {
                break;
            }
            ++loop;
            if (loop >= 32768) {
                printf("max=%d, min=%d\n", max, min);
                break;
            }
        }
    }
}

