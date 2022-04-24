all:
	clear
	make clean
	make main1
	make main2
main1:
	gcc -c main1.c
	gcc main1.o -o main1

main2:
	gcc -c main2.c
	gcc main2.o -o main2

clean:
	rm -f main1
	rm -f main2
	rm -f *.o