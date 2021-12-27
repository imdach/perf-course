#include <stdio.h>
#include <fcntl.h>

int main() {
    int fd = open("a.txt", O_RDONLY);
    printf("fd is %d\n", fd);
    getchar();
    return 0;
}