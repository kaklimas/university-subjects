#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

const int SIG = SIGUSR1;

void signal_handler(){
    printf("Process %d received signal!\n\n", (int) getpid());
}

void is_signal_pending(){
    // save pending signals in set
    sigset_t pending_set;
    sigpending(&pending_set);

    // check if our signal is in
    printf("\nMessage from process id %d:\n", (int) getpid());
    if (sigismember(&pending_set, SIG)){
        printf("Signal with id: %d is pending!\n\n", SIG);
    } else {
        printf("Signal with id: %d is not pending!\n\n", SIG);
    }
}

void mask_signal(){
    sigset_t blocket_set;

    sigemptyset(&blocket_set);

    sigaddset(&blocket_set, SIG);

    sigprocmask(SIG_BLOCK, &blocket_set, NULL);
}


int main(int argc, char* argv[]){
    if (argc != 2){
        printf("Wrong number of arguments!\n");
        return 1;
    }
    if (strcmp(argv[1], "ignore") == 0){
        signal(SIG, SIG_IGN);

    } else if (strcmp(argv[1], "handler") == 0){
        signal(SIG, signal_handler);

    } else if (strcmp(argv[1], "mask") == 0){

    } else if (strcmp(argv[1], "pending") == 0){
        mask_signal();
    } else {
        printf("Did not recognized command!\n");
    }



    // raise signal
    printf("Raising in main process....\n");
    raise(SIG);
    
    printf("Main process: ");
    is_signal_pending();

    // fork
    int child = fork();
    if (child == -1){
        printf("Fork failed!\n");
        return 1;
    }

    if (child == 0){

        if (strcmp(argv[1], "pending") != 0){
            printf("Raising in child process...\n\n");
            raise(SIG);
        }
        is_signal_pending();

        if (execl("./tmp", "tmp", NULL) == -1){
            printf("Cannot run tmp!\n");
            return 1;
        }
    }
    while (wait(NULL) != -1 && errno != ECHILD);

    return 0;
}