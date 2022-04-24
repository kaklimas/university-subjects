CC=gcc -Wall

all:
	make static

	echo "\n"

	make shared

static:
	make clean

	$(CC) -c lib.c
	ar rcs liblib.a lib.o

shared:
	make clean
	$(CC) -fPIC -c lib.c
	$(CC) -shared -fPIC -o liblib.so lib.o

	gcc -g -fPIC -Wall lib.c -shared -o liblib.so

clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
