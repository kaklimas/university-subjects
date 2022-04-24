#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
int test = 0;

struct result{
    int reg, dir, link, sock, blk, chr, u, fif;
} res;

struct stat file_data;

char* get_file_name(const char* abs_path){
    int len1 = strlen(abs_path);
    int i = len1 - 1, k = 0;
    while (abs_path[i] && abs_path[i] != '/'){
        i--;
    }
    i++;
    char* file_name = calloc(len1 - i, sizeof(char));
    for (i; i < len1; i++){
        file_name[k] = abs_path[i];
        k++;
    }
    return file_name;
}

char* merge_path(char* super_path, char* file_name){
    int len1 = strlen(super_path), len2 = strlen(file_name);
    int indx_2=0;

    char* result = calloc(len1 + len2 + 1, sizeof(char));
    result[len1] = '/';
    for (int i = 0; i < len1; i++){
        result[i] = super_path[i];
    }
    for (int i = len1 + 1; i < len1 + len2 + 1; i++){
        result[i] = file_name[indx_2];
        indx_2++;
    }

    return result;
}

void print(struct dirent* sd, char*dir_path){
    char* abs_path;
    if (strlen(sd->d_name) == 1 && sd->d_name[0] == '.'){
        abs_path = dir_path;
    } else{
        abs_path = merge_path(dir_path, sd->d_name);
    }
    lstat(abs_path, &file_data);

    time_t a_time = file_data.st_atim.tv_sec;
    time_t m_time = file_data.st_mtim.tv_sec;
    
    printf("\n\nFile name: %s\n", get_file_name(abs_path));
    
    
    printf("Absolute file path: %s\n", abs_path);
    printf("Number of hardlinks: %ld\n", file_data.st_nlink);
    switch (sd->d_type)
    {
    case DT_DIR:
        printf("File type: A directory\n");
        res.dir++;
        break;
    case DT_REG:
        printf("File type: A regular file\n");
        res.reg++;
        break;
    case DT_FIFO:
        printf("File type: A named pipe (FIFO)\n");
        res.fif++;
        break;
    case DT_CHR:
        printf("File type: A character device\n");
        res.chr++;
        break;
    case DT_BLK:
        printf("File type: A block device\n");
        res.blk++;
        break;
    case DT_LNK:
        res.link++;
        printf("File type: A symbolic link\n");
        break;
    case DT_SOCK:
        res.sock++;
        printf("File type: A UNIX domain socket\n");
        break;
    
    default:
        res.u++;
        printf("File type: File could not be determined (UNKNOWN)\n");
        break;
    }
    printf("Total size in bytes: %ld\n", file_data.st_size);
    printf("Last modification: %s", ctime(&m_time));
    printf("Last access: %s", ctime(&a_time));
    printf("\n");
}

void print_first(char*dir_path){
    struct dirent* sd;
    DIR* dir = opendir(dir_path);
    while (((sd=readdir(dir))!=NULL)){
        if (strlen(sd->d_name) == 1 && sd->d_name[0] == '.'){
            print(sd, dir_path);
            closedir(dir);
            return;
        }
    }
}
// depth first search function to browse tree

int dfs(char* abs_path){
    DIR* dir = opendir(abs_path);
    struct dirent* sd;  

    if (!dir){
        printf("Opening directory: %s failed", abs_path);
        return 1;
    }
    while ((sd=readdir(dir)) != NULL){
        if (sd->d_type==DT_DIR && !((strlen(sd->d_name) == 2 && sd->d_name[0] == '.' && sd->d_name[1] == '.') || (strlen(sd->d_name) == 1 && sd->d_name[0] == '.'))){
            dfs(merge_path(abs_path, sd->d_name));
            print(sd, abs_path);

        } else if (sd->d_type!=DT_DIR){
            print(sd, abs_path);
        }
    }
    closedir(dir);
    return 0;

}

int main(int argc, char* argv[]){
    
    if (argc == 2){
        char* dir_path = argv[1];
        
        char buf[PATH_MAX];
        char* real_path = realpath(dir_path, buf);
        if (!real_path){
            printf("Directory not found!\n");
            return 1;
        }
        int a = dfs(real_path);
        if (a == 1){
            printf("Directory not found!\n");
            return 1;
        }
        print_first(real_path);

        printf("\n\n---------------------------------\n");
        printf("Total number of files: %d\n", res.reg + res.dir + res.link + res.sock + res.blk + res.chr + res.u + res.fif);
        printf("Directories: %d\nRegular files: %d\nLinks: %d\nSockets: %d\nBlock devices: %d\nCharacter devices: %d\nNamed pipes(FIFO): %d\nUnknown: %d\n",
                res.dir, res.reg, res.link, res.sock, res.blk, res.chr, res.fif, res.u);
        return 0;
    } else {
        printf("Wrong number of arguments!\n");
        return 0;
    }
}