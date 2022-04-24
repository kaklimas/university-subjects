#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define RESULT_PATH "result.txt"

void wc_files(char* file_path){
	FILE * f; // input file
	FILE * r; // tmp output file

	char line[255];
	char* regex = " ";
	char* token;
	int lines=0, words=0,characters=0;

	f = fopen(file_path, "r");
	if (f == NULL){
		printf("File: %s not found\n", file_path);
		return;
	} else {
		fgets(line, 255, f);
		while (!(feof(f))){
			lines++;
			token = strtok(line, regex);
			while (token != NULL){
				characters += strlen(token);
				characters++;
				words++;
				token = strtok(NULL, regex);
			}
			characters--;
			fgets(line, 255, f);
		}
		fclose(f);

		r = fopen(RESULT_PATH, "a");
		if (r != NULL){
			fprintf(r, "%s:\nLines: %d | Words: %d | Characters: %d |\n\n", file_path, lines, words, characters);
			fclose(r);
		} else {
			system("touch $(RESULT_TXT)");
			r = fopen(RESULT_PATH, "a");
			fprintf(r, "%s:\nLines: %d | Words: %d | Characters: %d |\n\n", file_path, lines, words, characters);
			fclose(r);
		}
	}

}

int is_txt_file(char* string){
	int len = strlen(string);
	if (len > 4 && string[len-1] == 't' && string[len-2] == 'x' && string[len-3] == 't' && string[len-4] == '.'){
		return 1;
	}
	return 0;
}

void** create_table(int size){
	void** pointers = calloc(size, sizeof(void*));
	return pointers;
}

void remove_block(void** pointers, int index, int array_len){
	if (index > -1 && index < array_len){
		if ((char*)pointers[index] != NULL){
			free(pointers[index]);
			pointers[index] = NULL;
		} else {
			printf("Pointer of entered index doesn't point to any memory block!\n");
		}
	} else {
		printf("Entered index is incorrect!");
	}

}

int add_block(void** pointers, char* file_path, int array_len){
	int i=0, file_size;

	while (i < array_len && (char*)pointers[i] != NULL){
		i++;
	}
	if (i != array_len){
		FILE * f = fopen(file_path, "r");
		if (!f){
			printf("File not found!\n");
			return -1;
		}
		fseek(f, 0, SEEK_END);
		file_size = ftell(f);
		fseek(f, 0, SEEK_SET);
		pointers[i] = calloc(file_size, sizeof(char));
		fread(pointers[i], sizeof(char), file_size, f);
		fclose(f);
		return i;
	} else {
		printf("All pointers are taken!\n");
		return -1;
	}
}

