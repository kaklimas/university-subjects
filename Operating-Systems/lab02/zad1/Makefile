all:
	make clean
	make lib
	make sys
	clear

clean:
	rm -f *.o
	rm -f lib
	rm -f sys

lib:
	gcc -c lib.c
	gcc lib.o -o lib

sys:
	gcc -c sys.c
	gcc sys.o -o sys
	