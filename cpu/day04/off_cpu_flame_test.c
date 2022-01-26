#include <sys/epoll.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_EVENT 10

int main() {
    int ep_fd;
    ep_fd = epoll_create(MAX_EVENT);
    struct epoll_event events[MAX_EVENT];
    int ready;
    while (1) {
        ready = epoll_wait(ep_fd, events, MAX_EVENT, 20);
        usleep(30 * 1000);
    }
    close(ep_fd);
    return 0;
}