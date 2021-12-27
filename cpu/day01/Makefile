clean:
ifeq ($(shell uname), Darwin)
	find . -perm +111 -type f -exec rm '{}' \;
else
	find . -type f -executable -exec rm '{}' \;
endif
	rm -f *.o *.S

clone_test:clean
	gcc clone_test.c -o clone_test

fork_cow_test:clean
	gcc fork_cow_test.c -o fork_cow_test

pid_alloc_test:clean
	gcc pid_alloc_test.c -o pid_alloc_test

fd_test:clean
	gcc fd_test.c -o fd_test
