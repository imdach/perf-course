clean:
ifeq ($(shell uname), Darwin)
	find . -perm +111 -type f -exec rm '{}' \;
else
	find .  -maxdepth 1 -type f -executable -exec rm '{}' \;
endif
	rm -f *.o *.S

cache_line_test:clean
	gcc -std=c99 cache_line_test.c -o cache_line_test

array_test:clean
	gcc -std=c99 array_test.c -o array_test

cache_false_sharing:clean
	gcc -std=c99 cache_false_sharing.c -lpthread -o cache_false_sharing
	gcc -DENABLE=1 -std=c99 cache_false_sharing.c -lpthread -o cache_false_sharing_2


cache_line_size_test:clean
	gcc -std=c99 cache_line_size_test.c -lpthread -o cache_line_size_test

array_traverse:clean
	g++ array_traverse.cpp -o array_traverse