//
// Created by ahad on 8/21/18.
//
#include<iostream>
#include<string>
#include <fcntl.h>
#include <sys/stat.h>
#include "builtin.h"
#include "common.h"
#include <signal.h>

using namespace std;
char *cmd_exec[100];
char input_string[1024];
char pwd[1024];
char cur_dir[1024];
pid_t pid;
int len, flag;
int input_redirection, output_redirection;
char *input_command_file;
char *output_command_file;
int pipe_count;
int is_Background;
int child_pid;
struct sigaction sa;

void int_handler(int arg)
{
    printf("Child terminated.---..\n");
}

void clear_vars() {
    pid = 0;
    pwd[0] = '\0';
    flag = 0;
    pipe_count = 0;
    input_redirection = 0;
    output_redirection = 0;
    input_string[0] = '\0';

}
void sigint_handler(int sig_num)
{
    signal(SIGINT, sigint_handler);
    //printf("caught");
    exit(0);

}
void child_handler(int sig)
{
    pid_t pid;
    int status;
    //pid = wait(NULL);
    while((pid = waitpid(-1, &status, WNOHANG)) > 0);
    //cout << "abc";
}

void init_shell() {
    //clear();
    char shell[1000];
    if(getcwd(pwd, sizeof(pwd)) != NULL){
        strcpy(shell,"$ ");
        printf("%s",shell);
    } else
        perror("getcwd() error");

}
void change_directory() {
    std::string h = "/home";
    if(args[1] == NULL) chdir(h.c_str());
    else if(strcmp(args[1], "~\n") == 0) chdir(h.c_str());
    else if(strcmp(args[1], "~/\n") == 0) chdir(h.c_str());
    else if(strcmp(args[1], "\n") == 0 || strcmp(args[1], "") == 0) chdir(h.c_str());
    else if (chdir(args[1]) < 0)
        printf("bash: cd: %s: No such file or directory\n", args[1]);
}

void parent_directory() {
    //printf("working");
    if(getcwd(pwd, sizeof(pwd)) != NULL) printf("%s\n",pwd);
    else perror("getcwd() error");
}
void set_environment_vars() {
    char *temp[100];
    int cnt=1;
    temp[0] = strtok(args[1],"=");
    while((temp[cnt]=strtok(NULL,"="))!=NULL)
        cnt++;
    temp[cnt] = NULL;

    setenv(temp[0],temp[1], 1);
}
int parse_command(char *cmd_exec, int input, int first, int last) {
    char *copy_of_cmd_exec;
    copy_of_cmd_exec = strdup(cmd_exec);
    int m = 1;
    args[0]  = strtok(cmd_exec," ");
    while((args[m] = strtok(NULL," ")) != NULL)
        m++;
    args[m] = NULL;
    if(args[0] != NULL) {
        if (strcmp(args[0], "set") == 0)
        {
            set_environment_vars();
            return 1;
        }
        if(strcmp(args[0], "exit") == 0) {
            exit(0);
        }
        if(strcmp(args[0], "cd") == 0) {
            change_directory();
            //init_shell();
            return 1;
        }
        if(strcmp(args[0],"pwd\n") == 0) {
            parent_directory();
            return 1;
        }
        if(strchr(copy_of_cmd_exec, '&')) {
            args[m-1] = NULL;
            is_Background = 1;
	        copy_of_cmd_exec[strlen(copy_of_cmd_exec)-2] = '\0';
	        //cout << "inside";
        }

    }
    return command_execute(input, first, last, copy_of_cmd_exec);

}

char * space_removed(char *s) {
    int cnt=0;
    for(int i=0;s[i];i++) {
        if(s[i] != ' ')
            s[cnt++] = s[i];
    }
    s[cnt] = '\0';
    return s;

}
void input_output_file_processing(char *cmd_exec)
{
    char *temp[100];
    char *new_cmd_exec1;
    new_cmd_exec1=strdup(cmd_exec);
    int n=1;
    temp[0]=strtok(new_cmd_exec1,"<");
    while((temp[n]=strtok(NULL,">"))!=NULL)
        n++;
    temp[1]=space_removed(temp[1]);
    temp[2]=space_removed(temp[2]);
    input_command_file=strdup(temp[1]);
    output_command_file=strdup(temp[2]);
    int cnt=1;
    args[0]=strtok(temp[0]," ");
    while((args[cnt]=strtok(NULL," "))!=NULL)
        cnt++;
}
void input_file_processing(char *cmd_exec) {
    char *temp[100];
    char *temp_exec;
    temp_exec = strdup(cmd_exec);
    int n = 1;
    temp[0] = strtok(temp_exec,"<");
    while((temp[n] = strtok(NULL," ")) != NULL ) {
        n++;
    }
    temp[1] = space_removed(temp[1]);
    input_command_file = strdup(temp[1]);
    int cnt=1;
    args[0] = strtok(temp[0]," ");

    while((args[cnt] = strtok(NULL," "))!= NULL) {
        cnt++;
    }


}

void output_file_processing(char *cmd_exec) {
    char *temp[100];
    char *temp_exec;
    temp_exec = strdup(cmd_exec);
    temp[0] = strtok(temp_exec,">");;
    int cnt = 1;
    while((temp[cnt] = strtok(NULL,">")) != NULL) cnt++;
    temp[1] = space_removed(temp[1]);
    output_command_file = strdup(temp[1]);
    int n=1;
    args[0] = strtok(temp[0]," ");
    while((args[n] = strtok(NULL," ")) != NULL) {
        n++;
    }



}

int command_execute(int input, int first, int last, char *cmd_exec) {
    //struct sigaction sa;
    int status;
    signal(SIGCHLD, SIG_IGN);
    //int status;
    int pipe_fd[2], result, input_fd, output_fd;
    result = pipe(pipe_fd);
    if(result == -1) {
        perror("pipe");
        return 1;
    }
    pid = fork();
    if(pid == 0) {
        signal(SIGCHLD,SIG_DFL);
        if(first == 1 && last == 0 && input == 0) {
            dup2(pipe_fd[1], 1);

        }
        else if(first == 0 && last == 0 && input != 0) {
            dup2(input, 0);
            dup2(pipe_fd[1],1);
        }
        else {
            dup2(input, 0);
        }
        if(strchr(cmd_exec, '<') && strchr(cmd_exec,'>')) {
            input_redirection = 1;
            output_redirection = 1;
            input_output_file_processing(cmd_exec);

        }
        else if (strchr(cmd_exec, '<')) {
            input_redirection = 1;
            input_file_processing(cmd_exec);

        }
        else if (strchr(cmd_exec,'>')) {
            output_redirection = 1;
            output_file_processing(cmd_exec);

        }

        if(output_redirection == 1) {
            output_fd = creat(output_command_file, 0644);
            if(output_fd < 0) {
                fprintf(stderr, "Failed to open %s for writing\n", output_command_file);
                return(EXIT_FAILURE);
            }
            dup2(output_fd,1);
            close(output_fd);
            output_redirection = 0;
        }

        if(input_redirection == 1) {
            input_fd = open(input_command_file, O_RDONLY,0);
            if(input_fd < 0) {
                return EXIT_FAILURE;
            }
            dup2(input_fd,0);
            close(input_fd);
            input_redirection=0;
        }
        if(strcmp(strtok(cmd_exec," "),"myls") == 0) {
            char *temp[100];
            int n=1;
            extern char** environ;
            char *path = getenv("MYPATH");
            temp[0]=strtok(path,":");
            while((temp[n]=strtok(NULL,":"))!=NULL)
                n++;
            std::string tmp2 = "/myls";
            for(int i=0; i<n; i++) {
                //cout << (temp[i]+tmp2).c_str();
                if(!access((temp[i]+tmp2).c_str(),X_OK)) {
                    //cout << "inside";
                    if(execve((temp[i]+tmp2).c_str(), args, environ)<0) 
		            printf("error");
                }
            }

        }
        else {
            if(execvp(args[0], args)<0) printf("%s: command not found\n", args[0]);
        }
        exit(0);
    } else {
        if(!is_Background){
            signal(SIGCHLD,SIG_DFL);
            wait(&status);
            signal(SIGCHLD,child_handler);
        }
        else {
            signal(SIGCHLD,child_handler);
            is_Background = 0;
        }
    }

    if(last == 1) close(pipe_fd[0]);
    if(input != 0) close(input);
    close(pipe_fd[1]);
    return pipe_fd[0];
}
void pipe_command_execute() {
    int i;
    int n=1;
    int input, first;
    input = 0;
    first = 1;
    cmd_exec[0] = strtok(input_string,"|");
    while((cmd_exec[n] = strtok(NULL,"|")) != NULL)
        n++;
    cmd_exec[n] = NULL;
    pipe_count = n-1;
    for(i=0; i<=n; i++) {
        if(i == n) {
            input = 0;
        }
        else if(i == n-1) {
            input = parse_command(cmd_exec[i], input, first, 1);
        }
        else {
            input = parse_command(cmd_exec[i], input, first, 0);
            first = 0;

        }
    }
    return;
}

