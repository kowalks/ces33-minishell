#include <stdio.h>
#include <unistd.h>

#include "prompt.h"
#include "tokens.h"

#ifndef __COLORS__
#define __COLORS__
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define GRY   "\x1b[3;37m"
#define RESET "\x1B[0m"
#endif

#define EMPH GRN
#define EMPH2 CYN

void display_prompt() {
    char dir[1000];
    getcwd(dir, 1000);
    printf(EMPH "msh @ " GRY "%s" EMPH " $ " RESET, dir);
}

void info_prompt(pipe_t *p) {
    fprintf(stderr, EMPH2);
    fprintf(stderr, "  ➜ launched with pgid %d", p->pgid);
    fprintf(stderr, RESET "\n");
}

void welcome_message() {
    printf("                              \n" \
            "                 _        _   \n" \
            "                (_)      (_)  \n" \
            "      _ __ ___   _ _ __   _   \n" \
            "     | '_ ` _ \\ | | '_ \\ | |  \n" \
            "     | | | | | || | | | || |  \n" \
            "     | | |_| |_|| | | |_||_|  \n" \
            "  ___| |__   ___| | |         \n" \
            " / __| '_ \\ / _ \\ | |         \n" \
            " \\__ \\ | | |  __/ | |         \n" \
            " |___/_| |_|\\___|_|_|         \n" \
            "                              \n" \
            "Welcome to CES-33 minishell 🙂\n\n");
}