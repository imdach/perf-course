#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
int get_job_num(int argc, char *const *argv);
void *busy(void *args) {
    while (1);
}
int main(int argc, char *argv[]) {
    int num = get_job_num(argc, argv);
    printf("job num: %d\n", num);
    pthread_t threads[num];
    int i;
    for (i = 0; i < num; ++i) {
        pthread_create(&threads[i], NULL, busy, NULL);
    }
    for (i = 0; i < num; ++i) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}
int get_job_num(int argc, char *const *argv) {
    if (argc <= 1) {
        printf("illegal args\nusage ./fake_make -j8\n");
        exit(10);
    }
    int num;

    const char *optstring = "j:"; // 有三个选项-abc，其中c选项后有冒号，所以后面必须有参数
    int ret;
    ret = getopt(argc, argv, optstring);

    if (ret != 'j') {
        printf("illegal args\nusage ./fake_make -j8\n");
        exit(1);
    }
    num = atoi(optarg);
    return num;
}


