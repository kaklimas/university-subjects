#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int SIG_1;
int SIG_2;

int MODE;

int SIGNALS_CNT;
union sigval value;

void setup_sigaction(int id, void (*action)(int, siginfo_t*, void*))
{
    static struct sigaction act;
    act.sa_sigaction = action;
    act.sa_flags = SA_SIGINFO;
    sigemptyset(&act.sa_mask);

    sigaction(id, &act, NULL);
}

void sig1_handler(int sig, siginfo_t *info, void *ucontext) {
    SIGNALS_CNT++;
    if(MODE != 2) {
        kill(info->si_pid, SIG_1);
    } else {
        sigqueue(info->si_pid, SIG_1, value);
    }

}

void sig2_handler(int sig, siginfo_t *info, void *ucontext) {
    if(MODE != 2) {
        kill(info->si_pid, SIG_2);
    } else {
        sigqueue(info->si_pid, SIG_2, value);
    }
    printf("Caught signals from SENDER %d\n", SIGNALS_CNT);

    exit(0);
}


int main(int argc, char **argv) {
    printf("Hello from catcher!\n | ID: %d\n\n", getpid());


    if(argc != 2) {
        puts("WRONG NUMBER OF ARGUMENTS");
        return 1;
    }

    if (strcmp("KILL", argv[1]) == 0) {
        MODE = 1;
        SIG_1 = SIGUSR1;
        SIG_2 = SIGUSR2;
    } else if (strcmp("SIGQUEUE", argv[1]) == 0) {
        MODE = 2;
        SIG_1 = SIGUSR1;
        SIG_2 = SIGUSR2;
    } else if (strcmp("SIGRT", argv[1]) == 0) {
        MODE = 3;
        SIG_1 = SIGRTMIN + 1;
        SIG_2 = SIGRTMIN + 2;
    } else {
        puts("WRONG MODE");
        return 1;
    }

    SIGNALS_CNT = 0;
    value.sival_int = 0;

    setup_sigaction(SIG_1, sig2_handler);
    setup_sigaction(SIG_2, sig2_handler);
    
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIG_1);
    sigdelset(&mask, SIG_2);

    for(;;) {
        sigsuspend(&mask);
    }
    printf("\nEnd program.");
    return 0;
}