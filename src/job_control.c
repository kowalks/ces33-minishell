#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#include "tokens.h"

struct termios shell_tmodes;
pid_t shell_pgid;

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
    shell_pgid = getpid();
    setpgid(shell_pgid, shell_pgid);
    tcsetpgrp(STDIN_FILENO, shell_pgid);
    tcgetattr(STDIN_FILENO, &shell_tmodes);
}

void set_child_pid(pid_t pgid) {
    pid_t child_pid = getpid();
    if (!pgid)
        pgid = child_pid;
    setpgid(child_pid, pgid);
    // tcsetpgrp(STDIN_FILENO, pgid);
}

void set_parent_pid(pipe_t *p, cmd_t *cmd, pid_t child_pid) {
    cmd->pid = child_pid;
    if (!p->pgid)
        p->pgid = child_pid;
    setpgid(child_pid, p->pgid);
}

void put_in_foreground(pipe_t *p, int sigcont) {
    int wstatus;

    // Set pipe to foreground
    tcsetpgrp(STDIN_FILENO, p->pgid);

    // Send SIGCONT
    if (sigcont){
        tcsetattr(STDIN_FILENO, TCSADRAIN, &p->tmodes);
        kill(-(p->pgid), SIGCONT);
    }

    // Wait for job
    waitpid(WAIT_ANY, &wstatus, WUNTRACED);

    // Restore shell to foreground and restore modes
    tcsetpgrp(STDIN_FILENO, shell_pgid);
    tcgetattr(STDIN_FILENO, &p->tmodes);
    tcsetattr(STDIN_FILENO, TCSADRAIN, &shell_tmodes);
}

void put_in_background(pipe_t *p, int sigcont) {
    if (sigcont)
        kill(-(p->pgid), SIGCONT);
}

int fd_is_valid(int fd) { // thanks to https://stackoverflow.com/questions/12340695/how-to-check-if-a-given-file-descriptor-stored-in-a-variable-is-still-valid
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}

void check_valid_fd(){
    for (int fd=1; fd <= 255; fd++)
        if (fd_is_valid(fd))
            printf("opened fd: %d\n", fd);
}