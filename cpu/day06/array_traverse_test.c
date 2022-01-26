#include <time.h>
#include <stdio.h>
#include <stdlib.h>

long timediff(clock_t t1, clock_t t2) {
    long elapsed;
    elapsed = ((double) t2 - t1) / CLOCKS_PER_SEC * 1000;
    return elapsed;
}

#define N 2048

int main(int argc, char **argv) {
    int slowMode = atoi(argv[1]);

    char arr[N][N];
    clock_t start, end;
    if (slowMode) {
        start = clock();
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                arr[j][i] = 0;
            }
        }
        end = clock();
        printf("arr[j][i] mode, time: %ld\n", timediff(start, end));
    } else {
        start = clock();
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                arr[i][j] = 0;
            }
        }
        end = clock();
        printf("arr[i][j] mode, time: %ld\n", timediff(start, end));
    }
}