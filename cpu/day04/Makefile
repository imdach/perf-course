clean:
ifeq ($(shell uname), Darwin)
	find . -perm +111 -type f -exec rm '{}' \;
else
	find .  -maxdepth 1 -type f -executable -exec rm '{}' \;
endif
	rm -f *.o *.S

flame_test:clean
	gcc -std=c99 flame_test.c -lpthread -o flame_test
off_cpu_flame_test:clean
	gcc -std=c99 off_cpu_flame_test.c -o off_cpu_flame_test
