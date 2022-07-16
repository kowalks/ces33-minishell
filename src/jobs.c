#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "tokens.h"
#include "job_control.h"

#define MAXLEN 1000

typedef char* string;

// Generates all possible commands (e.g. cat, /bin/cat, /etc/bin/cat ...)
ll *generate_env_paths(string file) {
    string env = getenv("PATH");
    
    ll *list = tokenize(env, ":");
    ll *paths = linked_list();
    push(paths, file);
    while(size(list)) {
        string new_file = malloc(MAXLEN*sizeof(char));
        sprintf(new_file, "%s/%s", (char*) pop(list), file);
        push(paths, new_file);
    }

    free(list);
    return paths;
}

// Open text files for input / output 
void open_io_files(cmd_t *cmd) {
    int fd;
    if (cmd->in && cmd->fd[0] == STDIN_FILENO) { 
        fd = open(cmd->in, O_RDONLY, 0666);
        cmd->fd[0] = fd;
    }
    if (cmd->out && cmd->fd[1] == STDOUT_FILENO) { 
        fd = open(cmd->out, O_TRUNC | O_WRONLY | O_CREAT, 0666);
        cmd->fd[1] = fd;
    }
}


// Redirects STDOUT and STDIN to the one configured on cmd
void redirect_io(cmd_t *cmd) {
    if (cmd->fd[0] != STDIN_FILENO) { // this is the infile
        dup2(cmd->fd[0], STDIN_FILENO);
        close(cmd->fd[0]);
    }
    if (cmd->fd[1] != STDOUT_FILENO) { // this is the outfile
        dup2(cmd->fd[1], STDOUT_FILENO);
        close(cmd->fd[1]);
    }
}

void start_cmd(cmd_t *cmd) {
    open_io_files(cmd);
    redirect_io(cmd);
    restore_signals();

    ll *paths = generate_env_paths(cmd->file);
    
    // Generates all possible commands (e.g. cat, /bin/cat, /etc/bin/cat ...)
    while (size(paths)){
        cmd->file = cmd->argv[0] = pop(paths);
        execv(cmd->file, cmd->argv);
    }
    free(paths);
    perror("execv");
    exit(EXIT_FAILURE);
}

void start_pipe(pipe_t *p) {
    cmd_t *cmd = p->cmd;
    pid_t child_pid;

    // int wstatus;
    int pipefd[2], infile, outfile;

    // Sanity check: empty pipe or special commands
    if (p->size == 0)
        return;
    if (strcmp(cmd->file, "exit") == 0)
        exit(EXIT_SUCCESS);
    if(strcmp(cmd->file, "cd") == 0) {
        chdir(cmd->argv[1]);
        return;
    }

    // Main pipe loop
    infile = p->fd[0];
    for (int i = 0; i < p->size; i++) {
        cmd = p->cmd + i;

        if (i < p->size-1) {
            if (pipe(pipefd) < 0)
                perror ("pipe");
            outfile = pipefd[1];
        } else outfile = p->fd[1];

        // Setting io file descriptors
        cmd->fd[0] = infile;
        cmd->fd[1] = outfile;

        child_pid = fork();
        if (child_pid == 0) { // child
            start_cmd(cmd);
        } else if (child_pid > 0) { // parent
            cmd->pid = child_pid;
            if (!p->pgid)
                p->pgid = child_pid;
            setpgid(child_pid, p->pgid);
            // waitpid(child_pid, &wstatus, WUNTRACED);
        } else { // error
            perror("fork");
            exit(EXIT_FAILURE);
        }

        // Checks if fd is not the end of pipe and closes it
        if (infile != p->fd[0])
            close(infile);
        if (outfile != p->fd[1])
            close(outfile);
        infile = pipefd[0];
    }
    
    // log_pipe(p);
    put_in_foreground(p, 0);
}
