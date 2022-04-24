#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/times.h>
static struct tms tms_start;
static clock_t real_start;


int count(int i, double intervals_by_process, double step){
    // get right file name
    char file_name[30] = "file";
    char buffer[10];

    sprintf(buffer, "%d", i + 1);

    strcat(file_name, buffer);
    strcat(file_name, ".txt");
    
    // count data in interval

    FILE * f = fopen(file_name, "a");
    if (!f){
        printf("File cannot be opened!\n");
        return 1;
    }
    double bottom_border = i * intervals_by_process * step;
    double top_border = (i + 1) * intervals_by_process * step;
    double result = 0.0;

    for (double x=bottom_border+step; x < top_border+step; x += step){
        result += (4/(1+x*x));
    }
    fprintf(f, "%f", result);

    fclose(f);
    return 0;
}

void set_clock(){
	real_start = times(&tms_start);	
}

void get_times(double n, double step, double pi){
	FILE * f = fopen("result.txt", "a");
    if (!f){
        return;
    }
    struct tms tms_end;
	clock_t real_end = times(&tms_end);

	printf("\nCalculate for %d processes and interval width: %0.10f (intervals per process = %d)\nPI = %.12f\n\nReal time: %f seconds\nSystem time: %f seconds\nUser time: %f seconds\n\n\n",
        (int) n, step, (int)(1.0 / n / step) ,pi,
        (double)(real_end-real_start)/sysconf(_SC_CLK_TCK),
		(double)(tms_end.tms_stime - tms_start.tms_stime)/sysconf(_SC_CLK_TCK),
		(double)(tms_end.tms_utime - tms_start.tms_utime)/sysconf(_SC_CLK_TCK));
	    printf("\n");
    fprintf(f, "\n-----------------------------------\nCalculate for %d processes and interval width: %.12f\nPI = %.10f\n\nReal time: %f seconds\nSystem time: %f seconds\nUser time: %f seconds\n\n\n",
        (int)n, step, pi,
        (double)(real_end-real_start)/sysconf(_SC_CLK_TCK),
		(double)(tms_end.tms_stime - tms_start.tms_stime)/sysconf(_SC_CLK_TCK),
		(double)(tms_end.tms_utime - tms_start.tms_utime)/sysconf(_SC_CLK_TCK));
    fclose(f);
}


int main(int argc, char* argv[]){
    if (argc == 3){
        set_clock();
        // interval width
        char* ptr;
        double step;
        step = strtod(argv[1], &ptr);
        // number of processes
        double n = atoi(argv[2]);
        
        double interval_per_process = (1.0/step)/n;
        
        double pi = 0;
        FILE * f;

        pid_t child;
        int status;

        for (int i = 0; i < n; i++){
            child = fork();
            
            // if we are in child process
            if (child == 0){
                // every child process ends when it ends counting its intervals of data
                exit(count(i, interval_per_process, step));
            }
        }
        // parent process waits for childs to end
        while (wait(NULL) != -1 || errno != ECHILD);
        printf("End of childs working. Mommy start working!\n");

        printf("Sum data from files...\n");
        for (int i = 1; i < n + 1; i++){
            char file_name[30] = "file";
            char buffer[10];
            char* ptr;
            double i_value;
            int file_size;

            // get right file name
            sprintf(buffer, "%d", i);
            strcat(file_name, buffer);
            strcat(file_name, ".txt");
            
            // open right file
            f = fopen(file_name, "r");
            

            // get file size
            fseek(f, 0, SEEK_END);
            file_size = ftell(f);
            fseek(f, 0, SEEK_SET);

            // read file data and convert it into double type
            char val[file_size];
            fread(val, file_size, sizeof(char), f);
            i_value = strtod(val, &ptr);

            // sum up result
            pi += i_value;
            fclose(f);
            
            char command[10]="rm -f ";
            strcat(command, file_name);
            system(command);
        }
        pi = 1.0/(1/step) * pi;
        get_times(n, step, pi);
    } else {
        printf("Wrong number of arguments!\n");
    }
    return 0;
}