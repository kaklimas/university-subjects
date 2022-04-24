counted_files=result.txt

# COUNTING FILES

# 3 small files
test_01=01 wc_files files/file1.txt files/file2.txt files/file3.txt

# 4 medium files
test_02=02 wc_files files/file4.txt files/file5.txt files/file6.txt files/file7.txt

# 3 big files
test_03=03 wc_files files/file8.txt files/file9.txt files/file10.txt

# 7 files including small and medium
test_04=04 wc_files files/file1.txt files/file2.txt files/file3.txt files/file4.txt files/file5.txt files/file6.txt files/file7.txt

# 10 files (all kind)
test_05_0=05_0 wc_files files/file1.txt files/file2.txt files/file3.txt files/file4.txt files/file5.txt files/file6.txt files/file7.txt files/file8.txt files/file9.txt files/file10.txt

# 20 big files
test_05_1=05_1 wc_files files/file10.txt files/file10.txt files/file10.txt files/file10.txt files/file10.txt files/file10.txt files/file10.txt files/file10.txt files/file10.txt files/file10.txt files/file10.txt files/file10.txt files/file10.txt files/file10.txt files/file10.txt files/file10.txt files/file10.txt files/file10.txt files/file10.txt files/file10.txt


# SAVING & REMOVING BLOCKS IN MEMORY (in order)

# save & remove 3 small blocks
test_06=06 create_table 3 add_block files/file1.txt add_block files/file2.txt add_block files/file3.txt remove_block 0 remove_block 1 remove_block 2

# save & remove 4 medium blocks
test_07=07 create_table 4 add_block files/file4.txt add_block files/file5.txt add_block files/file6.txt add_block files/file7.txt remove_block 0 remove_block 1 remove_block 2 remove_block 3

# save & remove 3 large blocks
test_08=08 create_table 3 add_block files/file8.txt add_block files/file9.txt add_block files/file10.txt remove_block 0 remove_block 1 remove_block 2

# save & remove 7 blocks including medium and big
test_09=09 create_table 7 add_block files/file8.txt add_block files/file9.txt add_block files/file10.txt add_block files/file5.txt add_block files/file6.txt add_block files/file7.txt add_block files/file4.txt remove_block 0 remove_block 1 remove_block 2 remove_block 3 remove_block 4 remove_block 5 remove_block 6

# SAVING & REMOVING BLOCKS IN MEMORY (no-order)

# save & remove 3 small blocks
test_10=10 create_table 3 add_block files/file1.txt remove_block 0 add_block files/file2.txt add_block files/file3.txt remove_block 0 remove_block 1

# save & remove 4 medium blocks
test_11=11 create_table 4 add_block files/file4.txt add_block files/file5.txt add_block files/file6.txt remove_block 0 remove_block 1 add_block files/file7.txt remove_block 0 remove_block 2

# save & remove 3 large blocks
test_12=12 create_table 3 add_block files/file8.txt remove_block 0 add_block files/file9.txt remove_block 0 add_block files/file10.txt remove_block 0

# save & remove 7 blocks including medium and big
test_13=13 create_table 7 add_block files/file8.txt remove_block 0 add_block files/file9.txt remove_block 0 add_block files/file10.txt remove_block 0 add_block files/file5.txt remove_block 0 add_block files/file6.txt remove_block 0 add_block files/file7.txt remove_block 0 add_block files/file4.txt remove_block 0


all:
	make clean
	make static
	make shared
	make dll

static:
	@echo -----------------------------
	@echo STATIC TEST
	@echo
	gcc -Wall -c lib.c
	ar rcs liblib.a lib.o
	gcc -Wall test.c -o test -L./ -llib
	make tests
	make clean
	
shared:
	@echo -----------------------------
	@echo DYNAMIC TEST
	@echo
	gcc -Wall -c lib.c
	gcc -Wall -shared -fPIC -o liblib.so lib.o
	gcc test.c -Wall -o test -L. -llib -Wl,-rpath=`pwd`
	make tests
	make clean
	
dll:
	@echo -----------------------------
	@echo DYNAMIC TEST
	@echo
	gcc -Wall -c lib.c
	gcc -Wall -shared -fPIC -o liblib.so lib.o
	gcc -Wall testdll.c -o testdll -L. -ldl -D DLL
	make testsdll
	make clean
	
tests:
	@echo COUNTING FILES
	@echo -----------------------------
	@echo THREE SMALL FILES
	@echo
	./test $(test_01)
	rm $(counted_files)
	@echo -----------------------------
	@echo FOUR MEDIUM FILES
	@echo
	./test $(test_02)
	rm $(counted_files)
	
	@echo -----------------------------
	@echo THREE BIG FILES
	@echo
	./test $(test_03)
	rm $(counted_files)
	
	@echo -----------------------------
	@echo SEVEN FILES INCLUDING SMALL AND MEDIUM
	@echo
	./test $(test_04)
	rm $(counted_files)
	
	@echo -----------------------------
	@echo 20 BIG FILES
	@echo
	./test $(test_05_1)
	rm $(counted_files)
	
	@echo -----------------------------
	@echo TEN FILES ALL KIND
	@echo
	./test $(test_05_0)
	
	cat $(counted_files)
	rm $(counted_files)
	
	@echo -----------------------------
	@echo SAVING AND REMOVING BLOCKS FROM MEMORY in order
	@echo
	./test $(test_06)
	./test $(test_07)
	./test $(test_08)
	./test $(test_09)
	
	@echo -----------------------------
	@echo SAVING AND REMOVING BLOCKS FROM MEMORY no-order
	@echo
	./test $(test_10)
	./test $(test_11)
	./test $(test_12)
	./test $(test_13)

	make clean
	
testsdll:
	@echo COUNTING FILES
	@echo -----------------------------
	@echo THREE SMALL FILES
	@echo
	./testdll $(test_01)
	rm $(counted_files)
	@echo -----------------------------
	@echo FOUR MEDIUM FILES
	@echo
	./testdll $(test_02)
	rm $(counted_files)
	
	@echo -----------------------------
	@echo THREE BIG FILES
	@echo
	./testdll $(test_03)
	rm $(counted_files)
	
	@echo -----------------------------
	@echo SEVEN FILES INCLUDING SMALL AND MEDIUM
	@echo
	./testdll $(test_04)
	rm $(counted_files)
	
	@echo -----------------------------
	@echo 20 BIG FILES
	@echo
	./testdll $(test_05_1)
	rm $(counted_files)
	
	@echo -----------------------------
	@echo TEN FILES ALL KIND
	@echo
	./testdll $(test_05_0)
	
	cat $(counted_files)
	rm $(counted_files)
	
	@echo -----------------------------
	@echo SAVING AND REMOVING BLOCKS FROM MEMORY in order
	@echo
	./testdll $(test_06)
	./testdll $(test_07)
	./testdll $(test_08)
	./testdll $(test_09)
	
	@echo -----------------------------
	@echo SAVING AND REMOVING BLOCKS FROM MEMORY no-order
	@echo
	./testdll $(test_10)
	./testdll $(test_11)
	./testdll $(test_12)
	./testdll $(test_13)

	make clean
clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
	rm -f  test
	rm -f testdll
