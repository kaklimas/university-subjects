all:
	make clean
	gcc -c consumer.c producer.c
	gcc consumer.o -o consumer
	gcc producer.o -o producer

clean:
	rm -f consumer
	rm -f producer
	rm -f *.o

	