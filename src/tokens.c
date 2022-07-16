#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lists.h"
#include "tokens.h"

string trim_token(string s) {
    int right = strlen(s)-1;
    while(s[right]=='\n' && right>0)
        right--;
    s[right+1] = '\0';
    return s;
}

ll *tokenize(string s, string delim) {
    ll *list = linked_list();
    string token = strtok(s, delim);
    while (token != NULL) {
        push(list, trim_token(token));
        token = strtok(NULL, delim);
    }
    return list;
}

void print_cmd(cmd_t c) {
    printf("%s ", c.file);
    for (int i = 1; i < c.argc; i++)
        printf("[%s] ", c.argv[i]);
    if (c.in)
        printf("(< %s) ", c.in);
    if (c.out)
        printf("(> %s) ", c.out);
    printf("\n");
    printf("fd[0] = %d; fd[1] = %d\n", c.fd[0], c.fd[1]);
}

void print_pipe(pipe_t p) {
    for (int i=0; i < p.size; i++)
        print_cmd(p.cmd[i]);
    printf("\n");
    printf("p.fd[0] = %d; p.fd[1] = %d\n", p.fd[0], p.fd[1]);
}

cmd_t parse_cmd(string s) {
    ll *list = tokenize(s, " ");

    cmd_t cmd = {};
    cmd.file = (char*) front(list);
    cmd.argv = malloc((size(list)+1)*sizeof(string));

    while(size(list)) {
        string word = pop(list);
        if (strcmp(word, ">") == 0)
            cmd.out = pop(list);
        else if (strcmp(word, "<") == 0)
            cmd.in = pop(list);
        else
            cmd.argv[cmd.argc++] = word;
    }
    cmd.argv[cmd.argc] = NULL;
    free(list);

    if (!cmd.in) cmd.fd[0] = STDIN_FILENO;
    if (!cmd.out) cmd.fd[1] = STDOUT_FILENO;

    return cmd;
}

pipe_t parse_pipe(string s) {
    if (s[strlen(s)-1] == '\n')
        s[strlen(s)-1] = '\0';

    ll *list = tokenize(s, "|");

    pipe_t pipe;
    pipe.size = size(list);
    pipe.cmd = malloc(pipe.size*sizeof(cmd_t));

    for (int i = 0; i < pipe.size; i++)
        pipe.cmd[i] = parse_cmd(pop(list));

    free(list);

    pipe.fd[0] = STDIN_FILENO;
    pipe.fd[1] = STDOUT_FILENO;
    
    return pipe;
}