#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/mount.h>
#include <sys/wait.h>
#include "lxc_network/network.h"
#include <net/if.h>     // if_nametoindex
#include <arpa/inet.h>  // inet_pton

#define err_exit(msg) do { perror(msg); exit(EXIT_FAILURE);} while (0)

void init_network();
void start_network();


void set_rootdir(char *root_dir) {
    chdir(root_dir);
    chroot(".");
}

void start_cmd(char *cmd) {
    char *const child_args[] = {cmd, NULL};
    execv(cmd, child_args);
}

void set_mount() {
    mount("none", "/proc", "proc", 0, "");
    mount("none", "/sys", "sysfs", 0, "");
}

int pipefd[2];

char *cmd = "/bin/sh";
char *root_dir = "/home/ya/dev/docker_basic/busybox";
#define STACK_SIZE (1024 * 1024)
char child_stack[STACK_SIZE];

int cfs_period_us = 100000;
int cfs_quota_us = 50 * 1000;
int cpu_shares = 512;

int child_setup_func(void *args) {

    close(pipefd[1]);
    set_rootdir(root_dir); // chroot

    char ch;
    read(pipefd[0], &ch, 1); // wait for parent clone finish

    set_mount();
    start_network();
    start_cmd(cmd); // start_cmd
    return 0;
}

void write_file(char *path, char *data) {
    FILE *fp = NULL;
    fp = fopen(path, "w+");
    fputs(data, fp);
    fclose(fp);
}

void write_cgroup(pid_t pid) {
    char str[10];
    sprintf(str, "%d", pid);
    write_file("/sys/fs/cgroup/cpu/toy_docker/cgroup.procs", str);
    sprintf(str, "%d", cfs_period_us);
    write_file("/sys/fs/cgroup/cpu/toy_docker/cpu.cfs_period_us", str);
    sprintf(str, "%d", cfs_quota_us);
    write_file("/sys/fs/cgroup/cpu/toy_docker/cpu.cfs_quota_us", str);
    sprintf(str, "%d", cpu_shares);
    write_file("/sys/fs/cgroup/cpu/toy_docker/cpu.shares", str);
}

const char ip[] = "192.168.0.102";
const char broadcast[] = "192.168.100.255";
const char bridge_ip[] = "192.168.0.100";
const int network_mask = 8;

void start_network() {

    struct in_addr ip_addr;
    struct in_addr bcast_addr;
    struct in_addr gateway;

    if (inet_pton(AF_INET, ip, &ip_addr) < 0) {
        err_exit("inet_pton error");
    }

    if (inet_pton(AF_INET, broadcast, &bcast_addr) < 0) {
        err_exit("inet_pton error");
    }
    if (inet_pton(AF_INET, bridge_ip, &gateway) < 0) {
        err_exit("inet_pton error");
    }

    int if_index = if_nametoindex("eth0");
    if (if_index < 0) {
        err_exit("if_nametoindex fail");
    }

    if (lxc_ipv4_addr_add(if_index, &ip_addr, &bcast_addr, network_mask) < 0) {
        err_exit("lxc_ipv4_addr_add error");
    }
    lxc_netdev_up("lo");
    lxc_netdev_up("eth0");
    lxc_ipv4_gateway_add(if_index, &gateway);

    char mac[18];
    new_hwaddr(mac);
    setup_hw_addr(mac, "eth0");
}
char veth0buf[IFNAMSIZ] = "veth00X";
char veth1buf[IFNAMSIZ] = "veth01X";

char *veth0;
char *veth1;

int main() {
    if (pipe(pipefd) < 0) {
        err_exit("create pipe error.");
    }
    init_network();

    pid_t child_pid = clone(child_setup_func,
                            child_stack + STACK_SIZE,
                            SIGCHLD | CLONE_NEWUTS | CLONE_NEWNS | CLONE_NEWPID | CLONE_NEWNET,
                            NULL);
    if (child_pid < 0) {
        err_exit("clone child failed.");
    }
    write_cgroup(child_pid);

    lxc_netdev_move_by_name(veth1, child_pid, "eth0");
    close(pipefd[1]); // notify child

    waitpid(child_pid, NULL, 0);

    return 0;
}

void init_network() {

    veth0 = lxc_mkifname(veth0buf);
    veth1 = lxc_mkifname(veth1buf);
    // 创建一对网络设备, veth0 放到容器 namespace 下，veth1 连接到 bridge
    if (lxc_veth_create(veth0, veth1) < 0) {
        err_exit("lxc_veth_create");
    }
    setup_private_host_hw_addr(veth0);
    // 将 veth0 添加到网桥中
    lxc_bridge_attach("br0", veth0);
    lxc_netdev_up(veth0);
}
