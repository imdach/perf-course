#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("pid %d\n", getpid());
    int child_pid = fork();
    if (child_pid == 0) {
        printf("-----in child process:  %d\n", getpid());
        exit(0);
    } else {
        sleep(1000000);
    }
    return 0;
}