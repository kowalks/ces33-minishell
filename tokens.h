/**
 * Tokenization methods for command parsing
 * 
 * This header file declares common command and pipe data
 * structures that are used to construct 
 * 
 * Author: Guilherme Kowalczuk
 */

#ifndef __TOKENS_H__
#define __TOKENS_H__

#include "lists.h"

typedef char* string;

typedef struct cmd_t {
    string file;
    int argc;
    string *argv;
    string in;
    string out;
    int fd[2];
} cmd_t;

typedef struct pipe_t {
    int size;
    int fd[2];
    cmd_t *cmd;
} pipe_t;

// Return a linked list (see lists.h) with the partition of s in tokens
ll *tokenize(string s, string delim);

// Auxiliary function to print a single command (from cmd struct)
void print_cmd(cmd_t c);

// Auxiliary function to print a pipeline (from pipe struct)
void print_pipe(pipe_t p);

// Parse a string command into cmd struct
cmd_t parse_cmd(string s);

// Parse a string pipeline into pipe struct
pipe_t parse_pipe(string s);

#endif