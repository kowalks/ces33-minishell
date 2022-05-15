/**
 * Minishell - Milestone 1
 * 
 * Implementation for a simple minishell on Linux. 
 * 
 * Features:
 *     - Directory commands (try cd ..)
 *     - Exit command (try exit on terminal)
 *     - Output redirect (try echo hello > out.txt)
 *     - Pipe construction (try echo hello | grep h)
 *     - Env variable PATH for bin search (i.e. you can invoke
 *       a command with <name> instead of /bin/<name>)
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "tokens.h"
#include "prompt.h"

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

// Redirects STDOUT and STDIN to the one configured on cmd
void redirect_io(cmd_t cmd) {
    if (cmd.in) { 
        int fd = open(cmd.in, O_RDONLY, 0666);
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    if (cmd.out) { 
        int fd = open(cmd.out, O_TRUNC | O_WRONLY | O_CREAT, 0666);
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

int main () {
    char str[MAXLEN];

    welcome_message();
    display_prompt();
    while (fgets(str, sizeof(str), stdin)) {
        str[strlen(str)-1] = 0;

        pipe_t p = parse_pipe(str);

        int prev_fd[2], next_fd[2];
        pipe(next_fd);

        for (int i = 0; i < p.size; i++) {
            cmd_t cmd = p.cmd[i];

            if(strcmp(cmd.file, "cd") == 0) {
                chdir(cmd.argv[1]);
                continue;
            }
            else if (strcmp(cmd.file, "exit") == 0)
                exit(EXIT_SUCCESS);

            int wstatus;
            pid_t child_pid = fork();
            if (child_pid == 0) { // child one
                redirect_io(cmd);
                if (i!=0)
                    dup2(prev_fd[0], STDIN_FILENO);
                if (i != p.size-1)
                    dup2(next_fd[1], STDOUT_FILENO);

                // I can close all file descriptors because 
                // the important ones are copied on STDIN (1)
                // and STDOUT (2).
                close(prev_fd[0]); close(prev_fd[1]);
                close(next_fd[1]); close(next_fd[0]);

                // Generates all possible commands (e.g. cat, /bin/cat, /etc/bin/cat ...)
                ll *paths = generate_env_paths(cmd.file);
                while (size(paths)){
                    cmd.file = cmd.argv[0] = pop(paths);
                    execv(cmd.file, cmd.argv);
                }
                free(paths);
                perror("execv");
                exit(EXIT_FAILURE);
            } else if (child_pid > 0) { // parent one 
                if (i > 0)
                    close(prev_fd[0]);
                close(next_fd[1]);
                wait(&wstatus); // waits for this child
            } else {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            // swaps prev and next pipe file descriptors
            prev_fd[0] = next_fd[0];
            prev_fd[1] = next_fd[1];
            if (i < p.size-1)
                pipe(next_fd);
        }
        close(prev_fd[0]);
        display_prompt();
    }

    return 0;
}