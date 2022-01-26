
#include <stdio.h>
#include <pthread.h>

struct Point {
    long x;
#ifdef ENABLE
    long p_1;
    long p_2;
    long p_3;
    long p_4;
    long p_5;
    long p_6;
    long p_7;
#endif
    long y;
} point;

const int N = 500 * 1000 * 1000;

void *thread1(void *args) {
    for (int i = 0; i < N; i++) {
        point.x++;
    }
}

void *thread2(void *args) {
    for (int i = 0; i < N; i++) {
        point.y++;
    }
}

int main(int argc, char *argv[]) {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}

