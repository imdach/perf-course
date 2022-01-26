#include <stdio.h>
#include <pthread.h>
#include <sched.h>

static pthread_mutex_t mutex;
int thread_yields = 1000;
int num = 2;

void *routine(void *args) {
    int i;
    while (1) {
        for (i = 0; i < thread_yields; i++) {
            pthread_mutex_lock(&mutex);
            sched_yield();
            pthread_mutex_unlock(&mutex);
        }
    }
}
int main() {
    pthread_mutex_init(&mutex, NULL);

    pthread_t threads[num];
    for (int i = 0; i < num; ++i) {
        pthread_create(&threads[i], NULL, routine, NULL);
    }
    for (int i = 0; i < num; ++i) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}