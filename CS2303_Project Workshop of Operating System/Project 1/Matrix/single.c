#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

#define MAX_SIZE 128

int main()
{
    int a[MAX_SIZE][MAX_SIZE], c[MAX_SIZE][MAX_SIZE];
    int n, i, j, k;

    FILE *src = fopen("data.in", "r");
    if (!src) 
    {
        printf("Error: unable to open input file.\n");
        exit(-1);
    }

    fscanf(src, "%d", &n);
    for (i = 0; i < n; i++) 
    {
        for (j = 0; j < n; j++) 
        {
        fscanf(src, "%d", &a[i][j]);
        }
    }

    fclose(src);

    for (i = 0; i < n; i++) 
    {
        for (j = 0; j < n; j++) 
        {
            c[i][j] = 0;
            for (k = 0; k < n; k++) 
            {
                c[i][j] += a[i][k] * a[k][j];
            }
        }
    }

    FILE *dst = fopen("data.out", "w");
    if (!dst) 
    {
        printf("Error: unable to open output file.\n");
        exit(-1);
    }
    clock_t time_start, time_end;
    double elapsed;
    time_start = clock();

    for (i = 0; i < n; i++) 
    {
        for (j = 0; j < n; j++) 
        {
            fprintf(dst, "%d ", c[i][j]);
        }
            fprintf(dst, "\n");
    }

    time_end = clock();
    fclose(dst);
    elapsed = ((double) (time_end - time_start)) / CLOCKS_PER_SEC * 1000;
    printf("Time used: %f millisecond\n", elapsed);

    return 0;
}