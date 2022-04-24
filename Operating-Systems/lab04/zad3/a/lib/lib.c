#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include "lib.h"

void setup_sigaction(int id, void (*action)(int, siginfo_t*, void*))
{
    static struct sigaction act;
    act.sa_sigaction = action;
    act.sa_flags = SA_SIGINFO;
    sigemptyset(&act.sa_mask);

    sigaction(id, &act, NULL);
}


void send_messages(int process_id, int n, char* mode){
    if (strcmp(mode, "KILL") == 0){
        for (int i = 0; i < n; i++){
            kill(process_id, SIGUSR1);
        }       
        kill(process_id, SIGUSR2);
    } else if (strcmp(mode, "SIGQUEUE") == 0){
        for (int i = 0; i < n; i++){
            sigval_t signal_value = {i+1};
            sigqueue(process_id, SIGUSR1, signal_value);
        }
        sigval_t signal_value = {n+1};
        sigqueue(process_id, SIGUSR2, signal_value);
    } else {
        for (int i = 0; i < n; i++){
            kill(process_id, SIGRTMIN);
        }
        kill(process_id, SIGRTMIN + 1);
    }
}


sigset_t get_blocked_signals(int common_signal, int ending_signal){
    sigset_t blocked_signals;
    sigfillset(&blocked_signals);

    sigdelset(&blocked_signals, common_signal);
    sigdelset(&blocked_signals, ending_signal);
    sigdelset(&blocked_signals, SIGRTMIN);
    sigdelset(&blocked_signals, SIGRTMIN + 1);
    

    return blocked_signals;
}