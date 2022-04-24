#include <stdio.h>
#include <signal.h>
#include <unistd.h>

const int SIG = SIGUSR1;

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

int main(int argc, char* argv[]){
    printf("Child process after exec: \n");
    is_signal_pending();
    raise(SIG);
}