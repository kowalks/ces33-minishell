/**
 * Job launchers
 * 
 * This header file declares commands for pipeline and command launching
 * 
 * Author: Guilherme Kowalczuk
 */

#ifndef __JOBS_H__
#define __JOBS_H__

#include "tokens.h"

void open_io_files(cmd_t *cmd);

void redirect_io(cmd_t *cmd);

void start_cmd(cmd_t *cmd);

void start_pipe(pipe_t *p);

#endif