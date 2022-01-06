#define _GNU_SOURCE

#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/mount.h>
#include <net/if.h>     // if_nametoindex
#include <arpa/inet.h>  // inet_pton
#include <stdlib.h>

typedef int process_pid;
#define STACK_SIZE (1024 * 1024)

const char *cmd = "/bin/bash";
//char *cmd = "/bin/sh";

char child_stack[STACK_SIZE];

typedef struct container_config {
    char host_name[20];
    char root_dir[200];
    int cfs_quota_us;
    int cfs_period_us;
    int cpu_shares;
} container_config;

void set_hostname(struct container_config *config) {
    sethostname(config->host_name, strlen(config->host_name));
}

void start_cmd() {
    char *const child_args[] = {cmd, NULL};
    execv(cmd, child_args);
}

void set_rootdir(struct container_config *config) {
    chdir(config->root_dir);
    chroot(".");
}

void set_mount() {
    mount("none", "/proc", "proc", 0, (void *) "");
    mount("none", "/sys", "sysfs", 0, (void *) "");
}

void write_file(char *path, char *data) {
    FILE *fp = NULL;
    fp = fopen(path, "w+");
    fputs(data, fp);
    fclose(fp);
}

int child_setup_func(void *args) {
    struct container_config *config = (struct container_config *) args;
    set_hostname(config);
    set_rootdir(config);
    set_mount();
    start_cmd();
    return 0;
}

void write_cgroup(process_pid pid, struct container_config *config) {
    char str[10];
    sprintf(str, "%d", pid);
    write_file("/sys/fs/cgroup/cpu/tiny_docker/cgroup.procs", str);
    sprintf(str, "%d", config->cfs_period_us);
    write_file("/sys/fs/cgroup/cpu/tiny_docker/cpu.cfs_period_us", str);
    sprintf(str, "%d", config->cfs_quota_us);
    write_file("/sys/fs/cgroup/cpu/tiny_docker/cpu.cfs_quota_us", str);
    sprintf(str, "%d", config->cpu_shares);
    write_file("/sys/fs/cgroup/cpu/tiny_docker/cpu.shares", str);
}

static container_config config = {
        .host_name = "shifu.zhang",
        .root_dir="./rootfs_dir",
//        .root_dir="/home/ya/dev/docker_basic/busybox",
        .cfs_period_us=100000,
        .cfs_quota_us = -1,
        .cpu_shares=512
};

void start() {
    process_pid child_pid = clone(child_setup_func,
                                  child_stack + STACK_SIZE,
                                  SIGCHLD | CLONE_NEWUTS | CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWNET,
                                  (void *) &config);
    if (child_pid < 0) {
        perror("clone child failed.");
        return;
    }
    write_cgroup(child_pid, &config);
    waitpid(child_pid, NULL, 0);
}

int main() {
    start();
    return 0;
}