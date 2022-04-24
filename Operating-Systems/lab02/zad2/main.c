#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int BUFFER = 255;

static struct tms tms_start;
static clock_t real_start;

// TIME

void set_clock(){
	real_start = times(&tms_start);	
}

void get_times(){
	FILE * f = fopen("pomiar_zad_2.txt", "a");
    if (!f){
        return;
    }
    struct tms tms_end;
	clock_t real_end = times(&tms_end);

	printf("Real time: %f seconds\nSystem time: %f seconds\nUser time: %f seconds\n\n\n",
        (double)(real_end-real_start)/sysconf(_SC_CLK_TCK),
		(double)(tms_end.tms_stime - tms_start.tms_stime)/sysconf(_SC_CLK_TCK),
		(double)(tms_end.tms_utime - tms_start.tms_utime)/sysconf(_SC_CLK_TCK));
	    printf("\n");
    fprintf(f, "\nReal time: %f seconds\nSystem time: %f seconds\nUser time: %f seconds\n\n\n",
        (double)(real_end-real_start)/sysconf(_SC_CLK_TCK),
		(double)(tms_end.tms_stime - tms_start.tms_stime)/sysconf(_SC_CLK_TCK),
		(double)(tms_end.tms_utime - tms_start.tms_utime)/sysconf(_SC_CLK_TCK));
    fclose(f);
}

// GENERATE FILES
void generate_file(int lines, int bytes_per_line, char* file_path){
    FILE * file = fopen(file_path, "w");
    int file_size = lines * bytes_per_line;
    char content[file_size];
    char letters[] = "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < file_size; i++){
        if (i % 255 == 0 && i != 0){
            content[i] = '\n';
        } else if (i % 5 == 0){
            content[i] = ' ';
        } else {
            int ind = (rand() % (25 + 1));
            content[i] = letters[ind];
        }
    }
    fwrite(content, file_size, sizeof(char), file);
    fclose(file);
}

int count_lib(char* c, char* file_path){
    FILE * f = fopen(file_path, "r");
    int file_size, result=0;
    if (!f){
        printf("File not found");
        return 1;
    }
    fseek(f, 0, SEEK_END);
    file_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buff = calloc(file_size, sizeof(char));
    
    fread(buff, file_size, sizeof(char), f);
    fclose(f);
    
    for (int i = 0; i < file_size; i++){
        if (buff[i] == c[0]){
            result++;
        }
    }
    free(buff);
    return result;
}

void test(){
    system("touch large.txt");
    char* char_to_count = calloc(1, sizeof(char));
    char_to_count[0] = 'a';

    generate_file(30000, 255, "large.txt");
    
    printf("\n\nUsing library functions\n\n");
    printf("Test 1:\n");   
    set_clock();
    for (int i = 0; i < 10; i++){
        count_lib(char_to_count, "large.txt");
    }
    get_times();
    
    printf("Test 2:\n");
    set_clock();
    for (int i = 0; i < 100; i++){
        count_lib(char_to_count, "large.txt");
    }
    get_times();
    
    printf("Test 3:\n");
    set_clock();
    for (int i = 0; i < 1000; i++){
        count_lib(char_to_count, "large.txt");
    }
    get_times();
    
    printf("\n\nUsing system functions\n\n");
    
    printf("Test 1:\n");
    set_clock();
    for (int i = 0; i < 10; i++){
        count_lib(char_to_count, "large.txt");
    }
    get_times();
    
    printf("Test 2:\n");
    set_clock();
    for (int i = 0; i < 100; i++){
        count_lib(char_to_count, "large.txt");
    }
    get_times();
    
    printf("Test 3:\n");
    set_clock();
    for (int i = 0; i < 1000; i++){
        count_lib(char_to_count, "large.txt");
    }
    get_times();

    system("rm -f large.txt");
    free(char_to_count);
}

int count_sys(char* c, char* file_path){
    int file_fd = open(file_path, O_RDONLY), file_size, result=0;
    if (file_fd < 0){
        printf("File not found!\n");
        return 1;
    }
    file_size = lseek(file_fd, 0, SEEK_END);
    lseek(file_fd, 0, SEEK_SET);
    char* buff = calloc(file_size, sizeof(char));
    read(file_fd, buff, file_size);
    for (int indx = 0; indx < file_size; indx++){
        if (buff[indx] == c[0]){
            result++;
        }
    }
    free(buff);
    return result;
}


int main(int argc, char* argv[]){
    //test();
    FILE * f;
    int res1, res2;
    if (argc == 3){
        printf("\nUsing library functions\n\n");
        set_clock();
        res1 = count_lib(argv[1], argv[2]);
        f = fopen("pomiar_zad_2.txt", "a");
        fprintf(f, "Use library funcions\nFile name: %s\nResult: %d\nTimes:\n", argv[2], res1);
        fclose(f);
        get_times();

        printf("\n\nUsing system functions\n\n");
        set_clock();
        res2 = count_sys(argv[1], argv[2]);
        f = fopen("pomiar_zad_2.txt", "a");
        fprintf(f, "Use system funcions\nFile name: %s\nResult: %d\nTimes:\n", argv[2], res2);
        fclose(f);
        get_times();
    }
    printf("Result 1: %d\n", res1);
    printf("Result 2: %d\n", res2);
    return 0;
}