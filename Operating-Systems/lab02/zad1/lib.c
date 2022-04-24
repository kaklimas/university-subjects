#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/times.h>
#include <unistd.h>
int BUFFER = 256;
char RES_PATH[] = "pomiar_zad_1.txt";

static struct tms tms_start;
static clock_t real_start;

// GENERATE FILES
void generate_file(int lines, int bytes_per_line, char* file_path){
    FILE * file = fopen(file_path, "w");
    int file_size = lines * bytes_per_line;
    char content[file_size];
    char letters[] = "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < file_size; i++){
        if (i % (255*5) == 0 && i != 0){
            content[i-1] = '\n';
            content[i] = '\n';
        }
        else if (i % 255 == 0 && i != 0){
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

void set_clock(){
	real_start = times(&tms_start);	
}

void get_times(char* in, char* out){
	FILE * f = fopen(RES_PATH, "a");
    if (!f){
        return;
    }
    struct tms tms_end;
	clock_t real_end = times(&tms_end);

	printf("\nUsing library functions\nCopy files: %s - > %s:\nReal time: %f seconds\nSystem time: %f seconds\nUser time: %f seconds\n\n\n",
        in, out,
        (double)(real_end-real_start)/sysconf(_SC_CLK_TCK),
		(double)(tms_end.tms_stime - tms_start.tms_stime)/sysconf(_SC_CLK_TCK),
		(double)(tms_end.tms_utime - tms_start.tms_utime)/sysconf(_SC_CLK_TCK));
	    printf("\n");

    fprintf(f, "\nUsing library functions\nCopy files: %s - > %s:\n\nReal time: %f seconds\nSystem time: %f seconds\nUser time: %f seconds\n\n\n",
        in, out,
        (double)(real_end-real_start)/sysconf(_SC_CLK_TCK),
		(double)(tms_end.tms_stime - tms_start.tms_stime)/sysconf(_SC_CLK_TCK),
		(double)(tms_end.tms_utime - tms_start.tms_utime)/sysconf(_SC_CLK_TCK));
    fclose(f);
}

int copy(char* in, char* out){
    FILE * f_in = fopen(in, "r");
    FILE * f_out = fopen(out, "w");
    int file_size, line_len;
    char line[BUFFER];
    bool flag = false;

    if (!f_in){
        printf("Input file not found!\n");
        fclose(f_out);
        return 1;
    }

    while (!(feof(f_in))){
        fgets(line, BUFFER, f_in);
        flag = false;
        for (int i = 0; i < BUFFER; i++){
            if (line[i] == '\n' || !line[i]){
                line_len = i + 1;
                if (!line[i]){
                    line_len--;
                }
                break;
            }
            else if (line[i] != ' '){
                flag = true;
            }
        }
        if (flag){
            fwrite(line, line_len, sizeof(char), f_out);
        }
    }
    fclose(f_in);
    fclose(f_out);
    return 0;
}
int main(int argc, char* argv[]){
    int flag;
    if (argc != 3){
        char in[30];
        char out[30];
        printf("\nPlease insert paths to files: \n");
        scanf("%s\n%s", in, out);
        set_clock();
        flag = copy(in, out);
        if (flag == 0){
            get_times(in, out);
        }
        
    } else {
        set_clock();
        flag = copy(argv[1], argv[2]);
        if (flag == 0){
            get_times(argv[1], argv[2]);
        }
    }
    return 0;
}