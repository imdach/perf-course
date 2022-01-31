#include <stdio.h>
#include <ucontext.h>

static ucontext_t ctx[3];

static void foo(void) {
    printf("[coroutine] start foo\n");
    // 将当前 context 保存到 ctx[1]，切换到 bar 继续执行
    swapcontext(&ctx[1], &ctx[2]);
    printf("[coroutine] finish foo\n");
}

static void bar(void) {
    printf("[coroutine] start bar\n");
    // 将当前 context 保存到 ctx[2]，切换到 foo 继续执行
    swapcontext(&ctx[2], &ctx[1]);
    printf("[coroutine] finish bar\n");
}

int main(void) {
    char stack1[1024];
    char stack2[1024];

    getcontext(&ctx[1]);
    ctx[1].uc_stack.ss_sp = stack1;
    ctx[1].uc_stack.ss_size = sizeof(stack1);
    ctx[1].uc_link = &ctx[0]; // 执行完切换到 ctx[0]，也就是 cxt[2] 执行前
    makecontext(&ctx[1], foo, 0);

    getcontext(&ctx[2]);
    ctx[2].uc_stack.ss_sp = stack2;
    ctx[2].uc_stack.ss_size = sizeof(stack2);
    ctx[2].uc_link = &ctx[1];
    makecontext(&ctx[2], bar, 0);

    // 将当前 context 保存到 ctx[0]，切换到 ctx[2]
    swapcontext(&ctx[0], &ctx[2]);
    return 0;
}