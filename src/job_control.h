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

void ignore_signals(void);

void restore_signals(void);

void init_shell(void);

#endif