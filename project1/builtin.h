//
// Created by ahad on 8/21/18.
//

#ifndef SIMPLESHELL_BUILTIN_H
#define SIMPLESHELL_BUILTIN_H

#endif //SIMPLESHELL_BUILTIN_H
#include <dirent.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<vector>
using namespace std;

void init_shell();
void sigint_handler(int sig_num);
void clear_vars();
void pipe_command_execute();
int parse_command(char *cmd_exec, int input, int first, int last);
void change_directory();
void child_handler(int sig);
int command_execute(int input, int first, int last, char *cmd_exec);
void input_file_processing(char *cmd_exec);
void output_file_processing(char *cmd_exec);
void set_environment_vars();
void print_dir();
int built_in_command_handler(char **parsed);
int parse_pipe(char *input_args, char **piped_commands);
int process_string_args(char *input_args, char ***parsed);
void parse_space(char *input_args, char **parsed_commands);
int get_input();



