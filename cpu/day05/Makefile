clean:
ifeq ($(shell uname), Darwin)
	find . -perm +111 -type f -exec rm '{}' \;
else
	find .  -maxdepth 1 -type f -executable -exec rm '{}' \;
endif
	rm -f *.o *.S

context_switch_test:clean
	gcc -std=c99 context_switch_test.c -lpthread -o context_switch_test
