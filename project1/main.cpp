#include <iostream>
#include "builtin.h"
#include "common.h"
int main() {
    int status;
    struct sigaction sa;
   // char tmp[2] = {"\n"};
    getcwd(cur_dir,sizeof(cur_dir));
    signal(SIGINT,sigint_handler);
    while(1) {
        clear_vars();
        init_shell();
        //fgets(input_string,1024,stdin);
        if(fgets(input_string,1024,stdin) == NULL) exit(0);
        //if(!input_string) exit(0);
        if(strcmp(input_string,"\n") == 0) continue;
        len = strlen(input_string);
        input_string[len-1] = '\0';
        if(strcmp(input_string,"exit") == 0) {
            flag = 1;
            break;
        }
        pipe_command_execute();
        waitpid(-1, &status, WNOHANG);
    }
    if(flag == 1) {
        printf("\nBye");
        exit(0);
    }

    return 0;
}
