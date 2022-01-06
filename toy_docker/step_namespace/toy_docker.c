#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/wait.h>

#define err_exit(msg) do { perror(msg); exit(EXIT_FAILURE);} while (0)

void set_rootdir(char *root_dir) {
    chdir(root_dir);
    chroot(".");
}

void start_cmd(char *cmd) {
    char *const child_args[] = {cmd, NULL};
    execv(cmd, child_args);
}

char *cmd = "/bin/sh";
char *root_dir = "/home/ya/dev/docker_basic/busybox";
#define STACK_SIZE (1024 * 1024)
char child_stack[STACK_SIZE];

int child_setup_func(void *args) {
    set_rootdir(root_dir); // chroot
    start_cmd(cmd); // start_cmd
    return 0;
}

int main() {

//    SIGCHLD | CLONE_NEWUTS | CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWNET,
    pid_t child_pid = clone(child_setup_func,
                            child_stack + STACK_SIZE,
                            SIGCHLD | CLONE_NEWUTS | CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWNET,
                            NULL);

    if (child_pid < 0) {
        perror("clone child failed.");
        return 1;
    }
    waitpid(child_pid, NULL, 0);

    return 0;
}