#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main() {
    printf("before fork, pid=%d\n", getpid());
    pid_t childPid;
    switch (childPid = fork()) {
        case -1: {
            printf("fork error, %d\n", getpid());
            exit(1);
        }
        case 0: {
            printf("in child process, pid=%d\n", getpid());
            sleep(100000); // 子进程 sleep 不退出
            break;
        }
        default: {
            printf("in parent process, pid=%d, child pid=%d\n", getpid(), childPid);
            exit(0); // 父进程退出
        }
    }
    return 0;
}
