#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static void die(const char *error_msg) {
    perror(error_msg);
    exit(1);
}

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

int main() {
    set_rootdir(root_dir); // chroot

    start_cmd(cmd); // start_cmd
    return 0;
}