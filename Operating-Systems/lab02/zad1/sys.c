#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/times.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int BUFFER = 256;
char RES_PATH[] = "pomiar_zad_1.txt";

static struct tms tms_start;
static clock_t real_start;

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

	printf("\nUsing system functions\nCopy files: %s - > %s:\nReal time: %f seconds\nSystem time: %f seconds\nUser time: %f seconds\n\n\n",
        in, out,
        (double)(real_end-real_start)/sysconf(_SC_CLK_TCK),
		(double)(tms_end.tms_stime - tms_start.tms_stime)/sysconf(_SC_CLK_TCK),
		(double)(tms_end.tms_utime - tms_start.tms_utime)/sysconf(_SC_CLK_TCK));
	    printf("\n");

    fprintf(f, "\nUsing system functions\nCopy files: %s - > %s:\n\nReal time: %f seconds\nSystem time: %f seconds\nUser time: %f seconds\n\n\n",
        in, out,
        (double)(real_end-real_start)/sysconf(_SC_CLK_TCK),
		(double)(tms_end.tms_stime - tms_start.tms_stime)/sysconf(_SC_CLK_TCK),
		(double)(tms_end.tms_utime - tms_start.tms_utime)/sysconf(_SC_CLK_TCK));
    fclose(f);
}

int copy(char* in, char* out){
    int file_size, indx = 0, line_len=0, i;
    int in_fd = open(in, O_RDONLY);
    int out_fd = open(out, O_WRONLY);

    char line[BUFFER];
    int k;
    bool flag;

    if (in_fd < 0 || out_fd < 0){
        printf("Files not found not found!\n");
        return 1;
    }

    file_size = lseek(in_fd, 0, SEEK_END);
    lseek(in_fd, 0, SEEK_SET);

    char* buf = calloc(file_size, sizeof(char));
    read(in_fd, buf, file_size * sizeof(char));

    while (buf[indx]){
        flag = false;
        line_len=0;
        k = 0;

        while (buf[indx] != '\n' && buf[indx]){
            if (buf[indx] != ' '){
                flag = true;
            }
            line[k] = buf[indx];

            k++;
            line_len++;
            indx++;

        }
        if (buf[indx]){
            line[k] = buf[indx];
            line_len++;
        }
        if (flag){
            write(out_fd, line, line_len);
        }
        indx++;
    }


    free(buf);
    close(in_fd);
    close(out_fd);
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