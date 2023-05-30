/*
Prj1: Copy
A C program that forks two processes to copy file, one for reading
from a source file, and the other for writing into a destination file
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char *argv[]){
    if (argc != 4){
        printf("Error: Invalid command line arguments.\n");
        exit(-1);
    }

    char *src_path = argv[1];
    char *dest_path = argv[2];
    size_t buffer_size = atoi(argv[3]);
    
    if (buffer_size <= 0){
        printf("Error: Invalid buffer size.\n");
        exit(-1);
    }

    int mypipe[2];
    if (pipe(mypipe)){
        printf("Error: Pipe failed.\n");
        exit(-1);
    }

    pid_t pid;

    pid = fork();
    if (pid == -1){
    printf("Error: Failed to fork.\n");
    exit(-1);
    }
    
    clock_t start, end;
    double elapsed;
    start = clock();

    if (pid == 0){
        close(mypipe[0]);
        FILE *src;
        src = fopen(src_path, "r");
        if (src == NULL){
            printf("Error: Could not open source file '%s'.\n", src_path);
            close(mypipe[1]);
            exit(-1);
        }

        char *buffer = malloc(buffer_size);
        if (buffer == NULL){
            printf("Error: Could not create buffer.\n");
            close(mypipe[1]);
            exit(-1);
        }
        size_t bytes_read;
        while((bytes_read = fread(buffer, 1, buffer_size, src)) > 0){
            write(mypipe[1], buffer, bytes_read);
        }

        fclose(src);
        close(mypipe[1]);
        free(buffer);
    }
    else{
        close(mypipe[1]);
        FILE *dest;
        dest = fopen(dest_path, "w+");
        if (dest == NULL){
            printf("Error: Could not open destnation file '%s'.\n", dest_path);
            close(mypipe[0]);
            exit(-1);
        }

        char *buffer = malloc(buffer_size);
        if (buffer == NULL){
            printf("Error: Could not create buffer.\n");
            close(mypipe[0]);
            exit(-1);
        }
        size_t bytes_read;
        while((bytes_read = read(mypipe[0], buffer, buffer_size)) > 0){
            fwrite(buffer, 1, bytes_read, dest);
        }

        fclose(dest);
        close(mypipe[0]);
        free(buffer);

        wait(NULL);

        end = clock();
        elapsed = ((double)(end-start))/CLOCKS_PER_SEC*1000;
        printf("Time used: %f milliseconds.\n", elapsed);
    }

    return 0;
}