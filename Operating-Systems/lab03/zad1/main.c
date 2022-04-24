#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int print(int i){
    printf("Process id: %d | Iteration: %d\n", (int) getpid(), i + 1);
    return 0;
}
int main(int argc, char* argv[]){
    if (argc == 2){
        pid_t child;
        int status, n = atoi(argv[1]);

        for (int i = 0; i < n; i++){
            child = fork();
            
            // child process
            if (child == 0){
                exit(print(i));
            } 
        }
        while (wait(NULL) != -1 || errno != ECHILD);
    } else {
        printf("Wrong number of arguments!\n");
    }
    
    return 0;
}
