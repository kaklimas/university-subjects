#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int MODE;
int SIG_1;
int SIG_2;
int catcher;

int SIGNALS_CNT;
int SIGNALS_SENDED;
int SIGNALS_TO_SEND;

union sigval value;

void setup_sigaction(int id, void (*action)(int, siginfo_t*, void*))
{
    static struct sigaction act;
    act.sa_sigaction = action;
    act.sa_flags = SA_SIGINFO;
    sigemptyset(&act.sa_mask);

    sigaction(id, &act, NULL);
}

void send_sig1() {
    SIGNALS_SENDED++;
    if (MODE != 2) {
        kill(catcher, SIG_1);
    }
    else {
        sigqueue(catcher, SIG_1, value);
    }
}

void send_sig2() {
    if (MODE != 2) {
        kill(catcher, SIG_2);
    }
    else{
        sigqueue(catcher, SIG_2, value);
    }
}

void sig1_handler(int sig, siginfo_t *info, void *ucontext) {
    SIGNALS_CNT++;
    if(SIGNALS_SENDED < SIGNALS_TO_SEND) {
        send_sig1();
    } else {
        send_sig2();
    }
}

void sig2_handler(int sig, siginfo_t *info, void *ucontext) {
    printf("Caught signals from catcher %d\n", SIGNALS_CNT);
    exit(0);
}

int main(int argc, char **argv) {
    printf("Hello from sender!\n\n");
    if(argc != 4) {
        puts("WRONG NUMBER OF ARGUMENTS");
        return 1;
    }
    SIGNALS_CNT = 0;
    SIGNALS_SENDED = 0;


    catcher = strtol(argv[1], NULL, 10);
    SIGNALS_TO_SEND = strtol(argv[2], NULL, 10);

    if(strcmp(argv[3], "KILL") == 0) {
        MODE = 1;
        SIG_1 = SIGUSR1;
        SIG_2 = SIGUSR2;

    } else if(strcmp(argv[3], "SIGQUEUE") == 0) {
        MODE = 2;
        SIG_1 = SIGUSR1;
        SIG_2 = SIGUSR2;

    } else if(strcmp(argv[3], "SIGRT") == 0) {
        MODE = 3;
        SIG_1 = SIGRTMIN + 1;
        SIG_2 = SIGRTMIN + 2;

    } else {
        puts("WRONG MODE");
        return 1;
    }
    setup_sigaction(SIG_1, sig1_handler);
    setup_sigaction(SIG_2, sig2_handler);
    
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIG_1);
    sigdelset(&mask, SIG_2);

    send_sig1();
    sleep(1);

    for(;;) {
        sigsuspend(&mask);
    }
    printf("\nEnd program.\n");
    return 0;
}