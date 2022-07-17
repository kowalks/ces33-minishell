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

#include "job_control.h"
#include "jobs.h"
#include "prompt.h"
#include "tokens.h"

#define MAXLEN 1000

int main () {
    char str[MAXLEN];

    // Job control and signal handling
    init_shell();

    // Handler for pipes
    pipe_t pipes[MAXLEN];
    int i = 0;

    // Main loop
    welcome_message();
    display_prompt();
    while (fgets(str, sizeof(str), stdin)) {
        pipes[i] = parse_pipe(str);
        start_pipe(pipes+i);
        update_status(pipes, ++i);
        display_prompt();
    }

    return 0;
}