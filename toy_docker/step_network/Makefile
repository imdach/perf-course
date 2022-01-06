clean:
ifeq ($(shell uname), Darwin)
	find . -perm +111 -type f -exec rm '{}' \;

else
	find . -type f -executable -exec rm '{}' \;
endif
	rm -f *.o *.S

toy_docker:clean
	gcc -std=c99 toy_docker.c lxc_network/network.c lxc_network/network.h lxc_network/nl.c lxc_network/nl.h -o toy_docker