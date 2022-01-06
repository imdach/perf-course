clean:
ifeq ($(shell uname), Darwin)
	find . -perm +111 -type f -exec rm '{}' \;

else
	find . -type f -executable -exec rm '{}' \;
endif
	rm -f *.o *.S

toy_docker:clean
	gcc toy_docker.c -o toy_docker