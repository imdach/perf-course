#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>

#include "pqueue.h"

#define STACK_SIZE (32 * 1024)
struct cpu_context;

static struct schedule *_sched;

void swap_context(void *from_co, void *to_co) asm ("swap_context");
/*
*
1.before swap_context:
------
xxx
------  <-rsp

2. after call:
------
xxx
------
retaddr
------  <-rsp
*/
__asm__ (
"swap_context:\n\t"
/*
r12 r13 r14 r15 rip rsp rbx rbp
0   8   10  18  20  28  30  38
*/
// 根据 x86_64的传参规范，RDI, RSI, RDX, RCX, R8, R9 传递前六个参数
// swap_context(void *from_co, void *to_co)
// rdi->from_co | rsi->to_co
// 这个时候刚调用 swap_context 函数，rsp 正指向 retaddr, 将 rsp 先临时保存在 rdx 中，rcx、rdx 这两个寄存器不用保存
"mov 0x00(%rsp), %rdx\n\t"
// 将之前 RSP 的地址保存到 rcx 中
"lea 0x08(%rsp), %rcx\n\t"

// 保存所有的 callee 寄存器到 from_co 结构体
"mov %r12, 0x00(%rdi)\n\t" // r12 -> from_co[0]
"mov %r13, 0x08(%rdi)\n\t" // r13 -> from_co[1]
"mov %r14, 0x10(%rdi)\n\t" // r14 -> from_co[2]
"mov %r15, 0x18(%rdi)\n\t" // r15 -> from_co[3]
"mov %rdx, 0x20(%rdi)\n\t" // rdx -> from_co[4]  [retaddr]
"mov %rcx, 0x28(%rdi)\n\t" // rcx -> from_co[5]  [rsp]
"mov %rbx, 0x30(%rdi)\n\t" // rbx -> from_co[6]
"mov %rbp, 0x38(%rdi)\n\t" // rbp -> from_co[7]

// restore all the to_co registers
"mov 0x00(%rsi), %r12\n\t" // r12 <- to_co[0]
"mov 0x08(%rsi), %r13\n\t" // r13 <- to_co[1]
"mov 0x10(%rsi), %r14\n\t" // r14 <- to_co[2]
"mov 0x18(%rsi), %r15\n\t" // r15 <- to_co[3]
"mov 0x20(%rsi), %rax\n\t" // rax <- to_co[4]  [retaddr]
"mov 0x28(%rsi), %rcx\n\t" // rcx <- to_co[5]  [rsp]
"mov 0x30(%rsi), %rbx\n\t" // rbx <- to_co[6]
"mov 0x38(%rsi), %rbp\n\t" // rbp <- to_co[7]

"mov %rcx, %rsp\n\t"       // rsp
"jmpq *%rax\n\t"
);


typedef void(*my_func)(void *);

struct delay_task;
typedef struct delay_task node_t;

long now();

struct delay_task {
    long expire;
    struct coroutine *coroutine;
    int pos;
};

struct cpu_context {
    /**
     * r12
     * r13
     * r14
     * r15
     * retaddr
     * rsp
     * rbx
     * rbp
     */
    void *reg[8];
};


struct schedule {
    struct cpu_context main_context;
    struct coroutine *current;
    struct pqueue_t *queue;
};
struct coroutine {
    struct cpu_context context;
    char stack[STACK_SIZE];

    my_func func;
};


static int cmp_pri(pqueue_pri_t next, pqueue_pri_t curr);

static pqueue_pri_t get_pri(void *a);

static void set_pri(void *a, pqueue_pri_t pri);

static size_t get_pos(void *a);

static void set_pos(void *a, size_t pos);


struct schedule *coroutine_init() {
    struct schedule *sched = (struct schedule *) malloc(sizeof(struct schedule));
    printf("size: %d\n", sizeof(struct schedule));
    pqueue_t *queue = pqueue_init(100, cmp_pri, get_pri, set_pri, get_pos, set_pos);
    sched->queue = queue;
    printf("valid: %d\n", pqueue_is_valid(sched->queue));
    return sched;
}

struct coroutine *coroutine_new(struct schedule *sched, my_func func) {
    struct coroutine *c = malloc(sizeof(struct coroutine));
    c->func = func;
    c->context.reg[4] = func;
    c->context.reg[5] = c->stack + STACK_SIZE;
    return c;
}

void coroutine_resume(struct schedule *sched, struct coroutine *co) {
    sched->current = co;
    swap_context(&sched->main_context, &co->context);
}

void coroutine_yield(struct schedule *sched) {
    swap_context(&sched->current->context, &sched->main_context);
}


void coroutine_destroy(struct schedule *);


void delay(int ms) {
    struct delay_task *task = malloc(sizeof(struct delay_task));
    task->expire = ms + now();
    task->coroutine = _sched->current;
    pqueue_insert(_sched->queue, task);
    coroutine_yield(_sched);
}

void foo() {
    while (1) {
        static int i = 0;
        printf("[foo] run: %ld, i:%d\n", now() / 1000, ++i);
        delay(3000);
    }
}

void bar() {
    while (1) {
        printf("[bar] run: %ld\n", now() / 1000);
        delay(1000);
    }
}

long now() {
    struct timeval timeval;
    gettimeofday(&timeval, NULL);
    return timeval.tv_sec * 1000 + timeval.tv_usec / 1000;
}

void eventloop() {
    while (1) {
        usleep(1000);
        while (pqueue_size(_sched->queue) > 0) {
            node_t *top_item = pqueue_peek(_sched->queue);
            if (top_item == NULL) continue;
            if (now() > top_item->expire) {
                top_item = pqueue_pop(_sched->queue);
                struct coroutine *c = top_item->coroutine;
                free(top_item);
                coroutine_resume(_sched, c);
            } else {
                break;
            }
        }
    }
}

static int cmp_pri(pqueue_pri_t next, pqueue_pri_t curr) {
    return (next > curr);
}


static pqueue_pri_t get_pri(void *a) {
    return ((node_t *) a)->expire;
}


static void set_pri(void *a, pqueue_pri_t pri) {
    ((node_t *) a)->expire = pri;
}


static size_t get_pos(void *a) {
    return ((node_t *) a)->pos;
}

static void set_pos(void *a, size_t pos) {
    ((node_t *) a)->pos = pos;
}

int main() {

    _sched = coroutine_init();

    struct coroutine *c1 = coroutine_new(_sched, foo);
    struct coroutine *c2 = coroutine_new(_sched, bar);

    coroutine_resume(_sched, c1);
    coroutine_resume(_sched, c2);

    printf("%s\n", "in main");

    eventloop();
    return 0;
}
