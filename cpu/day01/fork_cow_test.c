#include <unistd.h>
#include <stdio.h>

int main() {
    int msg[1024 * 100] = {0};
    printf("parent[%d]: %p\n", getpid(), msg);

    int pid;
    pid = fork();

    if (pid == 0) { // 处理子进程
        printf("child[%d] : %p\n", getpid(), msg);
        getchar();
        sleep(2);
        msg[1024 * 10] = 1; // 修改 1024 * 10 处的值
        printf("child : %p\n", msg);
        sleep(1);
        msg[1024 * 20] = 1; // 修改 1024 * 20 处的值
        sleep(1);
        msg[1024 * 30] = 1; // 修改 1024 * 30 处的值
        sleep(1);
        msg[1024 * 40] = 1; // 修改 1024 * 40 处的值
        sleep(2);
        printf("address msg[1024 * 10]: %-10lx\n", (unsigned long) (msg + 1024 * 10));
        printf("address msg[1024 * 20]: %-10lx\n", (unsigned long) (msg + 1024 * 20));
        printf("address msg[1024 * 30]: %-10lx\n", (unsigned long) (msg + 1024 * 30));
        printf("address msg[1024 * 40]: %-10lx\n", (unsigned long) (msg + 1024 * 40));
        sleep(10000);
    } else {
        sleep(10000);
    }
}