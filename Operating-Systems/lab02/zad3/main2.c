#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

struct result{
    int reg, dir, link, u, cdir;
} res;

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

static int display_info(const char *file_path, const struct stat *file_data, int flags, struct FTW *ftwbuf){
    
    time_t a_time = file_data->st_atime;
    time_t m_time = file_data->st_mtime;
    
    printf("\n\nFile name: %s\n", get_file_name(file_path));
    
    
    printf("Absolute file path: %s\n", file_path);
    printf("Number of hardlinks: %ld\n", file_data->st_nlink);

    switch (flags)
    {
    case FTW_D:
        printf("File type: A directory\n");
        res.dir++;
        break;
    case FTW_F:
        printf("File type: A regular file\n");
        res.reg++;
        break;
    case FTW_DNR:
        printf("File type: A directory that cannot be read\n");
        res.cdir++;
        break;
    case FTW_SL:
        printf("File type: A symbolic link\n");
        res.link++;
        break;
    default:
        printf("File type: File could not be determined (UNKNOWN)\n");
        res.u++;
        break;
    }
    printf("Total size in bytes: %ld\n", file_data->st_size);
    printf("Last modification: %s", ctime(&m_time));
    printf("Last access: %s", ctime(&a_time));
    printf("\n");
    return 0;           /* To tell nftw() to continue */
}

int main(int argc, char *argv[]){
    int flags = FTW_PHYS;
    if (argc == 2){
        char* dir_path = argv[1];
        char buf[4096];
        char* real_path = realpath(dir_path, buf);
        nftw(real_path, display_info, 20, flags);
        printf("\n\n---------------------------------\n");
        printf("Total number of files: %d\n", res.reg + res.dir + res.link + res.u);
        printf("Directories: %d\nRegular files: %d\nLinks: %d\nUnknown: %d\n", res.dir, res.reg, res.link, res.u);

    } else {
        printf("Wrong number of arguments!\n");
        return 1;
    }
    return 0;

}
