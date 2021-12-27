#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char content[] = "hello, world";

    int fd = open("/dev/null", O_RDWR | O_CREAT);
    if (fd < 0) {
        return 1;
    }

    int index;
    int loop = 1000 * 1000 * 1000;
    for (index = 0; index < loop; ++index) {
        write(fd, content, sizeof(content));
    }

    close(fd);
}