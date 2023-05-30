#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>

#define MAX_SIZE 1024
#define num_threads 32

int A[MAX_SIZE][MAX_SIZE], B[MAX_SIZE][MAX_SIZE], C[MAX_SIZE][MAX_SIZE];

struct thread_arguments
{
    int first_parameter;
    int second_parameter;
    int return_value;
};

void* multiply(void* arg) //A*A
{
    // clock_t time_start, time_end;
    // double elapsed;
    // time_start = clock();
    struct thread_arguments* data = (struct thread_arguments*) arg;
    int start = data->first_parameter;
    int end = data->second_parameter;
    int n = data->return_value;
    for (int i = start; i < end; i++) 
    {
        for (int j = 0; j < n; j++) 
        {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) 
            {
                C[i][j] += A[i][k] * A[k][j];
            }
        }
    }
    // time_end = clock();
    // elapsed = ((double) (time_end - time_start)) / CLOCKS_PER_SEC * 1000;
    // printf("*Time used: %f millisecond\n", elapsed);
    pthread_exit(NULL);
}

void* multiply2(void* arg) //A*B
{
    struct thread_arguments* data = (struct thread_arguments*) arg;
    int start = data->first_parameter;
    int end = data->second_parameter;
    int n = data->return_value;
    for (int i = start; i < end; i++) 
    {
        for (int j = 0; j < n; j++) 
        {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) 
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    if(argc == 2)
    {
        int n = atoi(argv[1]);
        int i, j;
        for (i = 0; i < n; i++) 
        {
            for (j = 0; j < n; j++) 
            {
                A[i][j] = rand() % 60;
                B[i][j] = rand() % 60;
            }
        }

        pthread_t threads[num_threads];
        struct thread_arguments data[num_threads];

        int number = n / num_threads;
        int start = 0;
        int end;
        for (i = 0; i < num_threads-1; i++) 
        {
            end = start + number;
            data[i].first_parameter = start;
            data[i].second_parameter = end;
            data[i].return_value = n;
            int rc = pthread_create(&threads[i], NULL, multiply2, (void*) &data[i]);
            if (rc) 
            {
                printf("ERROR: return code from pthread_create(threads[%d]) is %d\n", i, rc);
                exit(1);
            }
            start = end;
        }

        end = n;
        data[num_threads-1].first_parameter = start;
        data[num_threads-1].second_parameter = n;
        data[num_threads-1].return_value = n;
        int rc = pthread_create(&threads[num_threads-1], NULL, multiply2, (void*) &data[num_threads-1]);
        if (rc) 
        {
            printf("ERROR: return code from pthread_create(threads[%d]) is %d\n", num_threads-1, rc);
            exit(1);
        }
        start = end;

        clock_t time_start, time_end;
        double elapsed;
        time_start = clock();

        for (i = 0; i < num_threads; i++) 
        {
            int rc = pthread_join(threads[i], NULL);
            if(rc)
            {
                printf("Error: return code from pthread_join(threads[%d]) is %d\n", i, rc);
                exit(1);
            }
        }

        time_end = clock();
        elapsed = ((double) (time_end - time_start)) / CLOCKS_PER_SEC * 1000;
        printf("Time used: %f millisecond\n", elapsed);

        FILE* dst = fopen("random.out", "w");
        if (!dst) 
        {
            printf("Error: unable to open output file.\n");
            exit(-1);
        }

        fprintf(dst, "%d\n", n);
        for (i = 0; i < n; i++) 
        {
            for (j = 0; j < n; j++) 
            {
                fprintf(dst, "%d ", A[i][j]);
            }
            fprintf(dst, "\n");
        }

        for (i = 0; i < n; i++) 
        {
            for (j = 0; j < n; j++) 
            {
                fprintf(dst, "%d ", B[i][j]);
            }
            fprintf(dst, "\n");
        }

        for (i = 0; i < n; i++) 
        {
            for (j = 0; j < n; j++) 
            {
                fprintf(dst, "%d ", C[i][j]);
            }
            fprintf(dst, "\n");
        }
        fclose(dst);
    }
    else
    {
        FILE* src = fopen("data.in", "r");
        if (!src) 
        {
            printf("Error: unable to open input file.\n");
            exit(-1);
        }

        int n;
        fscanf(src, "%d", &n);

        int i, j;
        for (i = 0; i < n; i++) 
        {
            for (j = 0; j < n; j++) 
            {
                fscanf(src, "%d", &A[i][j]);
            }
        }

        pthread_t threads[num_threads];
        struct thread_arguments data[num_threads];

        int number = n / num_threads;

        int start = 0;
        int end;
        for (i = 0; i < num_threads-1; i++) 
        {
            end = start + number;
            data[i].first_parameter = start;
            data[i].second_parameter = end;
            data[i].return_value= n;
            int rc = pthread_create(&threads[i], NULL, multiply, (void*) &data[i]);
            if (rc) 
            {
                printf("Error: return code from pthread_create(threads[%d]) is %d\n", i, rc);
                exit(1);
            }
            start = end;
        }

        end = n;
        data[num_threads-1].first_parameter = start;
        data[num_threads-1].second_parameter = n;
        data[num_threads-1].return_value = n;
        int rc = pthread_create(&threads[num_threads-1], NULL, multiply, (void*) &data[num_threads-1]);
        if (rc) 
        {
            printf("Error: return code from pthread_create(threads[%d]) is %d\n", num_threads-1, rc);
            exit(1);
        }
        start = end;

        clock_t time_start, time_end;
        double elapsed;
        time_start = clock();

        for (i = 0; i < num_threads; i++) 
        {
            int rc = pthread_join(threads[i], NULL);
            if(rc)
            {
                printf("Error: return code from pthread_join(threads[%d]) is %d\n", i, rc);
                exit(1);
            }
        }

        time_end = clock();
        elapsed = ((double) (time_end - time_start)) / CLOCKS_PER_SEC * 1000;
        printf("Time used: %f millisecond\n", elapsed);

        FILE* dst = fopen("data.out", "w");
        for (i = 0; i < n; i++) 
        {
            for (j = 0; j < n; j++) 
            {
                fprintf(dst, "%d ", C[i][j]);
            }
            fprintf(dst, "\n");
        }

        fclose(src);
        fclose(dst);
    }
}