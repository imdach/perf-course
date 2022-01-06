clean:
ifeq ($(shell uname), Darwin)
	find . -perm +111 -type f -exec rm '{}' \;
else
	find . -type f -executable -exec rm '{}' \;
endif
	rm -f *.o *.S

busy:clean
	gcc busy.c -lpthread -o busy
