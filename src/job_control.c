#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "tokens.h"

struct termios shell_tmodes;

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
    tcsetpgrp(STDIN_FILENO, shell_pgid);
    tcgetattr(STDIN_FILENO, &shell_tmodes);
}

void put_in_foreground(pipe_t *p, int sigcont) {
    int wstatus;

    // Set pipe to foreground
    tcsetpgrp(STDIN_FILENO, p->pgid);

    // Send SIGCONT
    if (sigcont)
        tcsetattr(STDIN_FILENO, TCSADRAIN, &p->tmodes);

    // Wait for job
    waitpid(WAIT_ANY, &wstatus, WUNTRACED);

    // Restore shell to foreground
    tcgetattr (STDIN_FILENO, &p->tmodes);
    tcsetattr (STDIN_FILENO, TCSADRAIN, &shell_tmodes);
}