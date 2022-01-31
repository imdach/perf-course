#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>

void foo(void) {
    printf("[coroutine] foo\n");
}

void bar(void) {
    printf("[coroutine] bar\n");
}

int main(void) {
    ucontext_t context1, context2;
    char stack1[1024];
    char stack2[1024];

    getcontext(&context1);
    context1.uc_stack.ss_sp = stack1;
    context1.uc_stack.ss_size = sizeof(stack1);
    context1.uc_link = NULL; // 结束运行
    makecontext(&context1, foo, 0); // context1 执行的函数为 foo

    getcontext(&context2);
    context2.uc_stack.ss_sp = stack2;
    context2.uc_stack.ss_size = sizeof(stack2);
    context2.uc_link = &context1; // 执行完切换到 context1
    makecontext(&context2, bar, 0); // context2 执行的函数为 bar

    printf("[main] Hello world\n");
    sleep(1);
    setcontext(&context2); // 切换到 context2 开始执行

    return 0;
}