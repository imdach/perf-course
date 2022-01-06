clean:
ifeq ($(shell uname), Darwin)
	find . -perm +111 -type f -exec rm '{}' \;
else
	find .  -maxdepth 1 -type f -executable -exec rm '{}' \;
endif
	rm -f *.o *.S

busy:clean
	gcc busy.c -lpthread -static -o busy
docker_main:clean
	gcc docker_main.c -lpthread -o docker_main
run_docker:
	sudo ./docker_main
