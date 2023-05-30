/*
Prj1: Shell (server ver.)
A shell-like program that illustrates how Linux spawns processes. 
It is as a server.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pwd.h>

#define MAX_INPUT_LEN 1024
#define MAX_NUM_ARGS 128

const char *CMD_EXIT = "exit";
const char *CMD_CD = "cd";
const char *CMD_PIPE = "|";

int parse_input(char *input, char **args);
void execute_cmd(char **args, int left, int right, int client_socket);
void getInfo(char *infoUser, char *infoPath);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(server_socket, 5);

    while (1)
    {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0)
        {
            perror("accept");
            continue;
        }

        pid_t pid = fork();

        if (pid == -1)
        {
            perror("fork");
            close(client_socket);
        }
        else if (pid == 0)
        {
            close(server_socket);

            char input[MAX_INPUT_LEN];
            char *args[MAX_NUM_ARGS];
            char infoUser[MAX_INPUT_LEN];
            char infoPath[MAX_INPUT_LEN];
            int num_args;
            ssize_t bytes_read;

            while (1)
            {
                getInfo(infoUser, infoPath);
                char prompt[MAX_INPUT_LEN];
                snprintf(prompt, sizeof(prompt), "\033[1;32m%s\033[0m\033[1m:\033[0m\033[1;36m%s\033[0m> ", infoUser, infoPath);
                write(client_socket, prompt, strlen(prompt));

                bytes_read = read(client_socket, input, sizeof(input) - 1);
                if (bytes_read <= 0)
                    break;
                input[bytes_read] = '\0';
                printf("received command: %s", input);

                num_args = parse_input(input, args);

                if (args[0] == NULL)
                {
                    continue;
                }
                else if (strcmp(args[0], CMD_EXIT) == 0) //exit
                {
                    break;
                }
                else if (strcmp(args[0], CMD_CD) == 0) // cd
                {
                    int ret = chdir(args[1]);
                    if (ret)
                    {
                        perror("cd");
                        write(client_socket, "Error: Unable to change directory\n", 34);
                    }
                    continue;
                }
                else
                {
                    execute_cmd(args, 0, num_args, client_socket);
                }
            }

            close(client_socket);
            exit(0);
        }
        else
        {
            close(client_socket);
        }
    }

    return 0;
}

int parse_input(char *input, char **args)
{
    char *token = strtok(input, " \n\r");
    int i = 0;
    while (token != NULL)
    {
        args[i++] = token;
        token = strtok(NULL, " \n\r");
    }
    args[i] = NULL;
    return i;
}

void execute_cmd(char **args, int left, int right, int client_socket)
{
    int pipeIdx = -1;
    for (int i = left; i < right; ++i)
    {
        if (strcmp(args[i], CMD_PIPE) == 0)
        {
            pipeIdx = i;
            break;
        }
    }
    if (pipeIdx == -1) //command with no pipe
    { 
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("Fork");
            return;
        }
        else if (pid == 0)
        {
            dup2(client_socket, STDOUT_FILENO);
            dup2(client_socket, STDERR_FILENO);
            execvp(args[left], &args[left]);
            perror("Execvp");
            exit(EXIT_FAILURE);
        }
        else
        {
            wait(NULL);
            return;
        }
    }
    else if (pipeIdx + 1 == right) //no more command after pipe symbol
    { 
        write(client_socket, "Wrong pipe parameters.\n", 23);
        return;
    }

    int fds[2];
    if (pipe(fds) == -1) //command with pipes
    {
        perror("Pipe");
        return;
    }
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Fork");
        return;
    }
    else if (pid == 0) //fork a sub process to do the cmd before pipe symbol
    {
        close(fds[0]);
        dup2(fds[1], STDOUT_FILENO);
        close(fds[1]);

        args[pipeIdx] = NULL;
        execvp(args[left], &args[left]);
        perror("Execvp");
        exit(EXIT_FAILURE);
    }
    else {
        wait(NULL);

        pid_t pid2 = fork(); //fork another sub process to do the cmd after pipe symbol
        if (pid2 == -1) {
            perror("Fork");
            return;
        }
        else if (pid2 == 0) {
            close(fds[1]);
            dup2(fds[0], STDIN_FILENO);
            close(fds[0]);
            execute_cmd(args, pipeIdx + 1, right, client_socket);
            // perror("Execvp");
            exit(EXIT_FAILURE);
        }
        else {
            close(fds[0]);
            close(fds[1]);
            wait(NULL);
            return;
        }
    }
}
void getInfo(char *infoUser, char *infoPath)
{
    char tmp[MAX_INPUT_LEN];

    struct passwd *pwd = getpwuid(getuid());
    strcpy(infoUser, pwd->pw_name);
    strcat(infoUser, "@");

    gethostname(tmp, MAX_INPUT_LEN);
    strcat(infoUser, tmp);

    getcwd(tmp, MAX_INPUT_LEN);
    strcpy(infoPath, tmp);
}
