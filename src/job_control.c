#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void set_signal(void sig(int)) {
    signal(SIGINT, sig);
    signal(SIGQUIT, sig);
    signal(SIGTSTP, sig);
    signal(SIGTTIN, sig);
    signal(SIGTTOU, sig);
    signal(SIGCHLD, sig);
}
void ignore_signals() {
    set_signal(SIG_IGN);
}

void restore_signals(void) {
    set_signal(SIG_DFL);
}
 
 void init_shell(void) {
    ignore_signals();
    pid_t shell_pgid = getpid();
    setpgid(shell_pgid, shell_pgid);
    tcsetpgrp (STDIN_FILENO, shell_pgid);
}