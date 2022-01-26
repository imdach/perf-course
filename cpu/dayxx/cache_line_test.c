//
// Created by ya on 2021/12/27.
//


#include <stdio.h>
#include <time.h>

#define SIZE 2048
long timediff(clock_t t1, clock_t t2) {
    long elapsed;
    elapsed = ((double) t2 - t1) / CLOCKS_PER_SEC * 1000;
    return elapsed;
}

int main() {
    char arr[SIZE][SIZE];
    clock_t start, end;

    start = clock();

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            arr[j][i] = 0;
        }
    }
    end = clock();
    printf("time diff: %d\n", timediff(start, end));
    return 0;
}