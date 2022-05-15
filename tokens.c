#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lists.h"

typedef char* string;

typedef struct cmd_t {
    string file;
    int argc;
    string *argv;
    string in;
    string out;
} cmd_t;

typedef struct pipe_t {
    int size;
    int fd[2];
    cmd_t *cmd;
} pipe_t;

ll *tokenize(string s, string delim) {
    ll *list = linked_list();
    string token = strtok(s, delim);
    while (token != NULL) {
        push(list, token);
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
}

void print_pipe(pipe_t p) {
    for (int i=0; i < p.size; i++)
        print_cmd(p.cmd[i]);
    printf("\n");
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
    return cmd;
}

pipe_t parse_pipe(string s) {
    ll *list = tokenize(s, "|");

    pipe_t pipe;
    pipe.size = size(list);
    pipe.cmd = malloc(pipe.size*sizeof(cmd_t));

    for (int i = 0; i < pipe.size; i++)
        pipe.cmd[i] = parse_cmd(pop(list));
    
    free(list);
    return pipe;
}