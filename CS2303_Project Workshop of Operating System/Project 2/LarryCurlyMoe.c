/*
Solution of Stooge Farmers Problem
*/
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

# define MAX_STEP 100

sem_t empty_holes;    //空洞的信号量
sem_t planted_seeds;  //种植的种子的信号量
sem_t filled_holes;   //填好土的信号量
sem_t shovel;         //铲子的信号量
int holes_count = 0;  //空洞的数量
int seeds_count = 0;  //种植的种子的数量
int holes_filled = 0; //填好土的数量

void *larry(void *arg);
void *moe(void *arg);
void *curly(void *arg);

int main(int argc, char *argv[])
{
    if (argc!=2){
        printf("Error: Invalid command line arguments.\n");
        exit(-1);
    }

    pthread_t larry_thread, moe_thread, curly_thread;
    int i;
    int max_holes=atoi(argv[1]);

    if (max_holes<1){
        printf("Error: Invalid number of max holes.\n");
        exit(-1);
    }
    sem_init(&empty_holes, 0, max_holes);
    sem_init(&planted_seeds, 0, 0);
    sem_init(&filled_holes, 0, 0);
    sem_init(&shovel, 0, 1);

    pthread_create(&larry_thread, NULL, larry, NULL);
    pthread_create(&moe_thread, NULL, moe, NULL);
    pthread_create(&curly_thread, NULL, curly, NULL);

    pthread_join(larry_thread, NULL);
    pthread_join(moe_thread, NULL);
    pthread_join(curly_thread, NULL);

    sem_destroy(&empty_holes);
    sem_destroy(&planted_seeds);
    sem_destroy(&filled_holes);
    sem_destroy(&shovel);

    return 0;
}

void *larry(void *arg)
{
    while (1) {
        sem_wait(&empty_holes);
        sem_wait(&shovel);
        holes_count++;
        if (holes_count<=MAX_STEP)
           printf("Larry digs another hole #%d\n", holes_count);
        sem_post(&shovel);
        sem_post(&planted_seeds);
        sleep(rand() % 3);
    }
    return NULL;
}

void *moe(void *arg)
{
    while (1) {
        sem_wait(&planted_seeds);
        seeds_count++;
        if (seeds_count<=MAX_STEP)
            printf("Moe plants a seed at hole #%d\n", seeds_count);
        sem_post(&filled_holes);
        sleep(rand() % 3);
    }
    return NULL;
}

void *curly(void *arg)
{
    while (1) {
        sem_wait(&filled_holes);
        sem_wait(&shovel);
        holes_filled++;
        if (holes_filled<=MAX_STEP)
            printf("Curly fills a planted hole #%d\n", holes_filled);
        sem_post(&shovel);
        sem_post(&empty_holes);
        sleep(rand() % 3);
        if (holes_filled>MAX_STEP){
            printf("End Run\n");
            exit(1);
        }
    }
}
