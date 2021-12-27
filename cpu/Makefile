clean:
ifeq ($(shell uname), Darwin)
	find . -perm +111 -type f -exec rm '{}' \;

else
	find . -type f -executable -exec rm '{}' \;
endif
	rm -f *.o *.S

cpu_01:clean
	gcc cpu_01.c -o cpu_01

cpu_02:clean
	gcc cpu_02.c -lpthread -o cpu_02

cpu_04:clean
	gcc cpu_04.c -lpthread -o cpu_04

cpu_05:clean
	gcc -std=c99 cpu_05.c -lpthread -o cpu_05

cpu_06:clean
	gcc -std=c99 cpu_06.c -lpthread -o cpu_06

cpu_orphan:clean
	gcc cpu_orphan.c -o cpu_orphan

cpu_zombie:clean
	gcc cpu_zombie.c -o cpu_zombie