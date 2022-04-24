#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>


#ifndef DLL
#include "lib.h"
#endif

static struct tms tms_start;
static clock_t real_start;

void set_clock(){
	real_start = times(&tms_start);	
}

void get_times(){
	struct tms tms_end;
	clock_t real_end = times(&tms_end);

	printf("Real time: %f seconds\nSystem time: %f seconds\nUser time: %f seconds\n", 			(double)(real_end-real_start)/sysconf(_SC_CLK_TCK),
		(double)(tms_end.tms_stime - tms_start.tms_stime)/sysconf(_SC_CLK_TCK),
		(double)(tms_end.tms_utime - tms_start.tms_utime)/sysconf(_SC_CLK_TCK));
	printf("\n");
}

int main(int argc, char* argv[]){
	#ifdef DLL
	void* handle = dlopen("./liblib.so", RTLD_LAZY);
	if (!handle){
		printf("Library not found");
		exit(1);
	}
	void (*wc_files)(char* file_path) = dlsym(handle, "wc_files");
	int (*is_txt_file)(char* string) = dlsym(handle, "is_txt_file");
	void** (*create_table)(int size) = dlsym(handle, "create_table");
	void (*remove_block)(void** pointers, int index, int array_len) = dlsym(handle, "remove_block");
	int (*add_block)(void** pointers, char* file_path, int array_len) = dlsym(handle, "add_block");
	
	#endif
	set_clock();
	printf("\nTest number: %s\n\n", argv[1]);

	int indx = 2, table_size;
	void** main_table;
	int table_exist = 1;

	while (indx < argc){
		if (strcmp(argv[indx], "create_table")==0){
			indx++;
			if (indx < argc){
				table_size = atoi(argv[indx]);
				main_table = create_table(table_size);
				table_exist = 0;
			} else {
				break;
			}
		} else if (strcmp(argv[indx], "add_block") == 0){
			indx++;
			if (indx < argc && table_exist == 0){
				add_block(main_table, argv[indx], table_size);
			} else if (indx == argc){
				break;
			}
		} else if (strcmp(argv[indx], "remove_block")==0){
			indx++;
			if (indx < argc && table_exist == 0){
				remove_block(main_table, atoi(argv[indx]), table_size);
			} else if (indx == argc){
				break;
			}
		} else if (strcmp(argv[indx], "wc_files")==0){
			indx++;
			while (indx < argc && is_txt_file(argv[indx]) == 1){
				wc_files(argv[indx]);
				indx++;
			}
		}

		indx++;
	}
	#ifdef DLL
	dlclose(handle);
	#endif
	if (table_exist == 0){
		for (int i = 0; i < table_size; i++){
			free(main_table[i]);
		}
		free(main_table);
	}
	get_times();
	return 0;
}
