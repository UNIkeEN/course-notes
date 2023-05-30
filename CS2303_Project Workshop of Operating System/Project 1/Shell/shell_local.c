/*
Prj1: Shell
A shell-like program that illustrates how Linux spawns processes. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <pwd.h>

#define MAX_INPUT_LEN 1024
#define MAX_NUM_ARGS 128

const char* CMD_EXIT = "exit";
const char* CMD_CD = "cd";
const char* CMD_PIPE = "|";

int parse_input(char* input, char** args);
void execute_cmd(char** args, int left, int right);
void getInfo(char* infoUser, char* infoPath);

int main(){
    char input[MAX_INPUT_LEN];
    char* args[MAX_NUM_ARGS];
    char infoUser[MAX_INPUT_LEN];
    char infoPath[MAX_INPUT_LEN];
    int num_args;

    while(1){
        getInfo(infoUser, infoPath);
        printf("\033[1;32m%s\033[0m\033[1m:\033[0m\033[1;36m%s\033[0m> ", infoUser, infoPath);
        fgets(input, sizeof(input), stdin);

        num_args = parse_input(input, args);

        if (args[0]==NULL) continue;
        else if (strcmp(args[0], CMD_EXIT) == 0) break;
        else if(strcmp(args[0], CMD_CD) == 0){
            int ret = chdir(args[1]);
            if (ret) perror("cd");
            continue;
        }
        else execute_cmd(args, 0, num_args);
    }

    return 0;
}

int parse_input(char* input, char** args) {
    char* token = strtok(input, " \n\r");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \n\r");
    }
    args[i] = NULL;
    return i;
}

void execute_cmd(char **args, int left, int right) {
    int pipeIdx = -1;
    for (int i=left; i<right; ++i) {
        if (strcmp(args[i], CMD_PIPE) == 0) {
            pipeIdx = i;
            break;
        }
    }
    if (pipeIdx == -1) { // 不含有管道命令
        pid_t pid = fork();
        if (pid == -1){
            perror("Fork");
            return;
        }
        else if (pid == 0){ 
            execvp(args[left], &args[left]);
            perror("Execvp");
            exit(EXIT_FAILURE);
        }
        else {wait(NULL); return;}
    } else if (pipeIdx+1 == right) { // 管道命令'|'后指令缺失
        printf("Wrong pipe parameters.\n");
        return;
    }

    int fds[2];
    if (pipe(fds) == -1) {
        perror("Pipe");
        return;
    }
    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork");
        return;
    } else if (pid == 0) {
        close(fds[0]);
        dup2(fds[1], STDOUT_FILENO);
        close(fds[1]);

        args[pipeIdx] = NULL;
        execvp(args[left], &args[left]);
        perror("Execvp");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);

        int stdin_save = dup(STDIN_FILENO); // Save the current stdin
        close(fds[1]);
        dup2(fds[0], STDIN_FILENO);
        close(fds[0]);

        execute_cmd(args, pipeIdx + 1, right);

        dup2(stdin_save, STDIN_FILENO); // Restore the saved stdin
        close(stdin_save);
    }
}

void getInfo(char* infoUser, char* infoPath){
    char tmp[MAX_INPUT_LEN];

    struct passwd* pwd = getpwuid(getuid());
    strcpy(infoUser, pwd->pw_name);
    strcat(infoUser, "@");
    
    gethostname(tmp, MAX_INPUT_LEN);
    strcat(infoUser, tmp);
    
    getcwd(tmp, MAX_INPUT_LEN);
    strcpy(infoPath, tmp);
}