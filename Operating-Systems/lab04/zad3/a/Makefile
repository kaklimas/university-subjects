all:
	make clean
	gcc -c sender.c 
	gcc -c catcher.c
	gcc -c lib/lib.c
	gcc sender.o lib.o -o sender
	gcc catcher.o lib.o -o catcher

clean:
	cd lib
	rm -f *.o
	cd ..
	rm -f *.o
	rm -f catcher
	rm -f sender