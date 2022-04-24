#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/file.h>
int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Template: ./producer [pipe_path] [row_no] [file_path] [characters_no]\n");
        return 1;
    }

    FILE *pipe;
    if (!(pipe = fopen(argv[1], "w"))){
        printf("Given pipe cannot be opened!\n");
        return 1;
    }
    flock(fileno(pipe), LOCK_EX);

    FILE *file;
    if (!(file = fopen(argv[3], "r"))){
        printf("Given file cannot be opened!\n");
        return 1;
    }
    flockfile(pipe);
    const int row_no = atoi(argv[2]);
    const int chars_no = atoi(argv[4]);

    fprintf(pipe, ":%d:", row_no);
    char *buffer = calloc(chars_no, sizeof(char));
    int c;
    while (c = fread(buffer, 1, chars_no, file) > 0) {
        sleep(rand() % 2 + 1);
        fprintf(pipe, "%s", buffer);
        for (int j = 0; j < chars_no-1;j++){
            buffer[j]='.';
        }
        fflush(pipe);
    }

    flock(fileno(pipe), LOCK_UN);
    funlockfile(pipe);
    free(buffer);
    fclose(file);
    fclose(pipe);
    
}