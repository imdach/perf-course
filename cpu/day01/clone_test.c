#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <string.h>
#include <sys/wait.h>

#define STACK_SIZE  (256 * 1024)

int child_func(void *arg) {
    char *buf = (char *) arg;
    strcpy(buf, "hello from child"); // 修改 buf 内容
    return 0;
}

int main(int argc, char **argv) {
    int clone_flags = SIGCHLD  ;
    // 如果第一个参数是 clone_vm，则给 clone_flags 增加 CLONE_VM 标记
    if (argc > 1 && !strcmp(argv[1], "clone_vm")) {
        clone_flags |= CLONE_VM;
    }
    char buf[] = "msg from parent";

    char *stack = malloc(STACK_SIZE);
    // int clone(int (*fn)(void *), void *stack, int flags, void *arg, ...
    int child_pid = clone(child_func, stack + STACK_SIZE, clone_flags, buf);
    if (child_pid < 0) exit(1); // clone 失败

    if (waitpid(child_pid, NULL, 0) < 0) {
        exit(1);
    }

    printf("buf:\"%s\"\n", buf);
    return 0;
}