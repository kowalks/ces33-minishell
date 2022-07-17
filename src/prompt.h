/**
 * Helper funcions to prompting
 * 
 * This header file declares some auxiliary functions to
 * the minishell. This funcions are purely aesthetic and
 * should not interfer on the minishell execution.
 * 
 * Author: Guilherme Kowalczuk
 */

#ifndef __PROMPT_H__
#define __PROMPT_H__

#include "tokens.h"

void welcome_message(void);

void display_prompt(void);

void info_prompt(pipe_t *p);

#endif