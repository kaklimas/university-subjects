#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#define PATH_MAX 4096

bool file_meets_conditions(char* file_path){
    int str_len = strlen(file_path);
    if (str_len > 4 && 
        file_path[str_len - 1] == 't' &&
        file_path[str_len - 2] == 'x' &&
        file_path[str_len - 3] == 't' &&
        file_path[str_len - 4] == '.'){
            return true;
        }
    return false;
}


void print_cutted_path(char* absolute_path, int curr_depth){
    curr_depth++;
    int indx = strlen(absolute_path) - 1;
    while (indx > -1 && curr_depth != 0){
        if (absolute_path[indx] == '/'){
            curr_depth--;
        }
        indx--;
    }
    
    if (indx == -1){
        printf("Something went wrong!\n");
        return;
    } else {
        indx++;
        printf("\nFile with given string found!\n"); 
        printf("Relative path: ");
        for (indx; indx < strlen(absolute_path); indx++){
            printf("%c", absolute_path[indx]);
        }
        printf("\nProcess ID: %d\n", (int)getpid());
    }

}


void look_for(char* string, char* absolute_file_path, int curr_depth){
    FILE * f = fopen(absolute_file_path, "r");
    int file_size, indx=0, str_len = strlen(string);
    int str_indx = 0;

    fseek(f, 0, SEEK_END);
    file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* buffer = calloc(file_size, sizeof(char));
    fread(buffer, file_size, sizeof(char), f);
    fclose(f);

    while (indx < file_size){
        str_indx = 0;
        if (buffer[indx] == string[str_indx]){
            int i = indx;
            while (i < file_size && str_indx < str_len && buffer[i] == string[str_indx]){
                i++;
                str_indx++;    
            }
            if (str_indx == str_len){
                // found
                print_cutted_path(absolute_file_path, curr_depth);
                printf("Given string: %s\n", string);
                free(buffer);
                return;
            }
        }
        indx++;
    }

    free(buffer);
    return;
}


int dfs(char* absolute_path, char* to_find, int max_depth, int curr_depth)
{   
    DIR * dir;
    struct dirent* sd;
    pid_t child;
    if (curr_depth > max_depth){
        return 0;
    }

    dir = opendir(absolute_path);
    if (!dir){
        printf("Given directory cannot be opened! (%s)\n", absolute_path);
        return 1;
    }


    while ((sd=readdir(dir)) != NULL){
        // avoid parent dir and grand parent dir (. ..)
        if (strcmp(sd->d_name, ".") == 0 || strcmp(sd->d_name, "..") == 0){
                continue;
            }
        
        // make new absolute path
        char new_abs[strlen(absolute_path) + strlen(sd->d_name) + 1];
        sprintf(new_abs, "%s/%s", absolute_path, sd->d_name);
        
        if (sd->d_type==DT_DIR){
            child = fork();

            if (child == 0){
                char max_d[10];
                char curr_d[10];

                sprintf(max_d, "%d", max_depth);
                sprintf(curr_d, "%d", curr_depth);


                char *arg[] = {"main", new_abs, to_find, max_d, curr_d, NULL};
                execv("./main", arg);
            }
        } else if (sd->d_type == DT_REG && file_meets_conditions(sd->d_name)){
            look_for(to_find, new_abs, curr_depth);
        }
    }
    while (wait(NULL) != -1 && errno != ECHILD);
    closedir(dir);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 4){
        // 1 dir_path | 2 to_find | 3 max_depth

        int max_depth = atoi(argv[3]);
        int depth = 1;
        
        char absolute_path[PATH_MAX];
        realpath(argv[1], absolute_path);
        

        dfs(absolute_path, argv[2], max_depth, depth);

    } else if (argc == 5){
        // 1 dir_path | 2 to_find | 3 max_depth | 4 curr_depth
        int max_depth = atoi(argv[3]);
        int curr_depth = atoi(argv[4]) + 1;

        dfs(argv[1], argv[2], max_depth, curr_depth);

    } else {
        printf("Wrong number of arguments!\n");
        return 0;
    }

    return 0;
}