//
// Created by ahad on 9/29/18.
//

#include <signal.h>

#ifndef SIMPLESHELL_COMMON_H
#define SIMPLESHELL_COMMON_H

#endif //SIMPLESHELL_COMMON_H
extern char *cmd_exec[100];
static char *args[1024];
extern char input_string[1024];
extern char pwd[1024];
extern char cur_dir[1024];
extern pid_t pid;
extern int len, flag;
extern int input_redirection, output_redirection;
extern char *input_command_file;
extern char *output_command_file;
extern int pipe_count;
extern int is_Background;