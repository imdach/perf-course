#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        // 此处是子进程的代码分支
    } else if (pid > 0) {
        // 此处是父进程的代码分支
    } else if (pid < 0) {
        // fork 失败，比如进程 ID 耗尽等
    }
    return 0;
}
