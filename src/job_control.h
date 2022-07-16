/**
 * Helper funcions to jb controlling
 * 
 * This header file declares some auxiliary functions to
 * the minishell.
 * 
 * Author: Guilherme Kowalczuk
 */

#ifndef __JOB_CONTROL_H__
#define __JOB_CONTROL_H__

#include "tokens.h"

void ignore_signals(void);

void restore_signals(void);

void init_shell(void);

void put_in_foreground(pipe_t *p, int sigcont);

#endif