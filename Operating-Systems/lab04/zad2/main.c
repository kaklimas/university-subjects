#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

void signal_handler(int sig, siginfo_t* info, void* ucontext){
    printf("\nSignal info: \n");
    printf("- Signal number: %d\n", info->si_signo);
    printf("- The signal has been sent by process %d\n", info->si_pid);

}


void signal_handler_usr1(int sig, siginfo_t* info, void* ucontext){
    printf("\nSignal info: \n");
    printf("- Signal number: %d\n", info->si_signo);
    printf("- The signal has been sent by process %d\n", info->si_pid);
    printf("- Real user ID of sending process %d\n", info->si_uid);
}

void signal_handler_usr2(int sig, siginfo_t* info, void* ucontext){
    printf("\nSignal info: \n");
    printf("- Signal number: %d\n", info->si_signo);
    printf("- The signal has been sent by process %d\n", info->si_pid);
    printf("- Custom integer data sent %d\n", info->si_value.sival_int);
}


void signal_handler_child(int sig, siginfo_t* info, void* ucontext){
    printf("\nSignal info: \n");
    printf("- Signal number: %d\n", info->si_signo);
    printf("- Child pid: %d\n", info->si_pid);
    printf("- The signal has been sent by process %d\n", info->si_pid);
    printf("- Child exit status: %d\n", info->si_status);
}

void setup_sigaction(int signalIdx, int flags, void (*function)(int, siginfo_t*, void*)){
    static struct sigaction act;
    act.sa_sigaction = function;
    act.sa_flags = flags;
    sigemptyset(&act.sa_mask);

    sigaction(signalIdx, &act, NULL);
}

int main(int argc, char* argv[]){
    struct sigaction sa;
    

    // test SA_SIGINFO
    printf("\n ---- Test SA_SIGINFO ----\n");

    setup_sigaction(SIGUSR1, SA_SIGINFO, signal_handler_usr1);
    setup_sigaction(SIGUSR2, SA_SIGINFO, signal_handler_usr2);
    setup_sigaction(SIGCHLD, SA_SIGINFO, signal_handler_child);

    // first handler
    raise(SIGUSR1);
    // second handler
    sigval_t signal_value = {15};
    sigqueue(getpid(), SIGUSR2, signal_value);
    // third handler
    pid_t child = fork();
    if (child == 0){
        // Raising SIGCHLD from child process...
        exit(0);
    }
    while (wait(NULL) != -1 && errno != ECHILD);

    // test SA_NODEFER
    printf("\n\nTest NODEFER\n");
    setup_sigaction(SIGUSR1, SA_NODEFER, signal_handler);

    raise(SIGUSR1);

    printf("\n\nTest SA_RESTART\n");
    setup_sigaction(SIGUSR1, SA_RESTART, signal_handler);

    raise(SIGUSR1);
}












