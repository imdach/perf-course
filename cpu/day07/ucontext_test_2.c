#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>

void foo(void) {
    printf("[coroutine] foo\n");
}

int main(void) {
    ucontext_t context;
    char stack[1024];

    // 初始化 ucontext_t
    getcontext(&context);
    context.uc_stack.ss_sp = stack;
    context.uc_stack.ss_size = sizeof(stack);
    context.uc_link = NULL;
    // 设置 ucontext_t
    makecontext(&context, foo, 0);

    printf("[main] Hello world\n");
    sleep(1);
    setcontext(&context);
    return 0;
}