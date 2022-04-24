

void setup_sigaction(int signal, void (*action)(int, siginfo_t*, void*));

void send_messages(int process_id, int n, char* mode);

sigset_t get_blocked_signals(int common_signal, int ending_signal);