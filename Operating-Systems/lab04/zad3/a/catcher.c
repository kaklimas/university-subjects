#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include "./lib/lib.h"

int sender_id, received_signals = 0;
bool waiting_for_signals = true;
char* MODE;

void signal_handler(int sig, siginfo_t* info, void* ucontext){
    sender_id = info->si_pid;
    
    // check which mode is running
    if (info->si_value.sival_int != 0){
        MODE = "SIGQUEUE";
    } else if (sig == SIGUSR1 || sig == SIGUSR2){
        MODE = "KILL";
    } else {
        MODE = "SIGRT";
    }

    printf("Received signal of number %d | ", info->si_signo);
    if (strcmp(MODE, "SIGQUEUE") == 0){
        printf("Signal value: %d\n", info->si_value.sival_int);
    } else {printf("\n");}

    if (sig == SIGUSR2 || sig == SIGRTMIN + 1){
        waiting_for_signals = false;
    } else {
        received_signals++;
    }   
}

int main(int argc, char* argv[]){

    printf("Hello from catcher! | PID: %d\n\n", getpid());

    setup_sigaction(SIGUSR1, signal_handler);
    setup_sigaction(SIGUSR2, signal_handler);
    setup_sigaction(SIGRTMIN, signal_handler);
    setup_sigaction(SIGRTMIN + 1, signal_handler);
    

    sigset_t blocked_signals = get_blocked_signals(SIGUSR1, SIGUSR2);
    
    
    while (waiting_for_signals){
        sigsuspend(&blocked_signals);    
    }

    printf("\nCatcher received %d signals.\n", received_signals);

    printf("\nPrepare for sending signals...\n\n");
    send_messages(sender_id, received_signals, MODE);

    printf("\nEnd program.\n");
    return 0;
}