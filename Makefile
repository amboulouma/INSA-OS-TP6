main: main.c bag.c bag.h
	gcc -O3 -pthread -Wall bag.c main.c -o $@


clean:
	rm -f main
