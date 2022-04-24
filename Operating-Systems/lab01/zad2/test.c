#include <stdio.h>
#include "lib.h"
#include <string.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>

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
	if (table_exist == 0){
		for (int i = 0; i < table_size; i++){
			free(main_table[i]);
		}
		free(main_table);
	}
	get_times();
	return 0;
}
