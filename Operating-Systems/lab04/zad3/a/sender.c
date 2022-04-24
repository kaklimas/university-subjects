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
    if (argc != 4){
        printf("Wrong number of arguments in sender!\n");
        exit(1);
    }
    // 1 - n   2 - catcher_id    3  mode   
    printf("\nHello from sender! | PID: %d\nSender starts sending signals...\n\n", getpid());
    int n = atoi(argv[1]);
    int catcher_id = atoi(argv[2]);
    MODE = argv[3];


    // sending
    send_messages(catcher_id, n, MODE);

    printf("\nSending ended. Waiting for response....\n");
    // receiving
    setup_sigaction(SIGUSR1, signal_handler);
    setup_sigaction(SIGUSR2, signal_handler);
    setup_sigaction(SIGRTMIN, signal_handler);
    setup_sigaction(SIGRTMIN + 1, signal_handler);
    
    sigset_t blocked_signals = get_blocked_signals(SIGUSR1, SIGUSR2);
    
    while (waiting_for_signals){
        sigsuspend(&blocked_signals);       
    }
    printf("\nSender received back %d/%d signals.\n", received_signals, n);

    printf("\nEnd program.\n");
    return 0;
}