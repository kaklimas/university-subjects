#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/file.h>
#define BUFFOR_SIZE 1024
#define BUFFOR 4096

int getMax(char* buffer){
    int i = 0, k=0, max = 0;
    while (buffer[i]){
        if (buffer[i]==':'){
			i++;
			k = i;
			while (buffer[k] != ':'){
				k++;
			}
			char* number = calloc(k-i, 1);
			k = 0;
			while (buffer[i] != ':'){
				number[k] = buffer[i];
				k++; i++;
			}
			int num = atoi(number);
			if (max < num){
				max = num;
			}
			i++;
		}
		i++;
		
    }
    return max;
}

void appendFile(char** messages, int* rows_no, FILE* file, int chars_no, int lines, int bytes_per_line){
    int i=0, k=1;
    char line[BUFFOR];

	int file_size = lines * bytes_per_line;
    char content[file_size];

	int curr_mess_indx;
	char free_line[bytes_per_line];
	
	for (int p=0; p < bytes_per_line;p++){
		free_line[p]='.';
	}
	free_line[bytes_per_line-1] = '\n';
    
    for (int i = 0; i < lines; i++){
		// check whether is message on position
		int k = 0;
		curr_mess_indx = -1;
		while (rows_no[k]){
			if (rows_no[k] == i+1){
				curr_mess_indx = k;
				break;
			}
			k++;
		}
		
		if (curr_mess_indx != -1){
    		char* buffor = calloc(chars_no, 1);

			int x = 0, y;
			while (messages[curr_mess_indx][x]){
				for (int k =0;k<chars_no;k++){
					buffor[k]='.';
				}
				y = 0;
				while (messages[curr_mess_indx][x] && y < chars_no){
					buffor[y] = messages[curr_mess_indx][x];
					x++; y++;
				}
				fwrite(buffor, 1, chars_no, file);
			}
			free(buffor);
			fwrite("\n", 1, 1, file);

		} else {
			fwrite(free_line, 1, bytes_per_line, file);
		}

        
    }
}


int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Template: ./consumer [pipe_path] [file_path] [characters_per_read]\n");
        return 1;
    }

    FILE *pipe;
    if (!(pipe=fopen(argv[1], "r"))){
        printf("Given pipe cannot be opened!\n");
        exit(1);
    }
    //flock(fileno(pipe), LOCK_EX);

    FILE *file;
    if (!(file=fopen(argv[2], "w+"))){
        printf("Given file cannot be opened!\n");
        exit(1);
    }
    const int chars_no = atoi(argv[3]);
    int pipe_size, i=0, curr_row;
    
    char* buffer = calloc(BUFFOR, sizeof(char));
    fread(buffer, sizeof(char), BUFFOR, pipe);


    
    int k;
    
    int max_row = getMax(buffer);
    int counter = 0;
    while (buffer[i]){
        if (buffer[i] == ':'){
            counter++;
        }
        i++;
    }
    counter /= 2;
    i = 0;

    char** messages = calloc(counter, sizeof(char*));
    for (int k=0;k<counter;k++){
        messages[k]=calloc(BUFFOR, sizeof(char));
    }
    int* rows = calloc(counter, sizeof(int));
    int indx=0;
    
    while (buffer[i]){
        char message[BUFFOR_SIZE];
        for (int k = 0; k < BUFFOR_SIZE;k++){
            message[k]=' ';
        }
        if (buffer[i] ==':'){
            i++;
            k = i;
            while (buffer[k] != ':'){
                k++;
            }
            char *num = calloc(k-i, 1);
            k = 0;
            while (buffer[i] != ':'){
                num[k] = buffer[i];
                i++; k++;
            }
            curr_row = atoi(num);
            i++;
            free(num);
        }
        k = 0;
        while (buffer[i] && buffer[i] != ':' ){
            message[k] = buffer[i];
            i++; k++;
        }
        for (int j = 0; j < k; j++){
            messages[indx][j] = message[j];
        }
        rows[indx] = curr_row;
        indx++;
        
    }
    appendFile(messages, rows, file, chars_no, max_row, 100);

    for (int j=0; j<counter;j++){
        free(messages[j]);
    }
    free(messages);
    free(buffer);
    fclose(file);
    fclose(pipe);
}