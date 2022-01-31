clean:
ifeq ($(shell uname), Darwin)
	find . -perm +111 -type f -exec rm '{}' \;
else
	find .  -maxdepth 1 -type f -executable -exec rm '{}' \;
endif
	rm -f *.o *.S

ucontext_test:clean
	gcc ucontext_test.c -o ucontext_test

ucontext_test_2:clean
	gcc ucontext_test_2.c -o ucontext_test_2

ucontext_test_3:clean
	gcc ucontext_test_3.c -o ucontext_test_3

ucontext_test_4:clean
	gcc ucontext_test_4.c -o ucontext_test_4

coroutine_test:clean
	gcc coroutine_test.c coroutine/coroutine.c coroutine/coroutine.h -o coroutine_test

 my_coroutine:clean
	gcc my_coroutine.c pqueue.c pqueue.h -o my_coroutine
