/*
Solution of Faneuil Hall Problem
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>
#include <assert.h>

#define MAX_THREAD 500
#define MAX_CHAIR 300

int num_cur_thread = 0;

typedef struct node
{
    int data;
    struct node *next;
}Node;

typedef struct linklist		//single linklist queue to store immgrants
{
    struct node *head;
    int currentLen;
}Linklist;

void init(Linklist *list){
    list->currentLen=0;
    Node* head=(Node*)malloc(sizeof(Node));
    assert(head);
    head->next = NULL;
    list->head = head;
}

void enqueue(Linklist *list, int x){
    Node* p=list->head;
    int i=list->currentLen;
    while(i-->0) p=p->next;

    Node* newnode=(Node*)malloc(sizeof(Node));
    assert(newnode);
    newnode->next=NULL;
    newnode->data=x;
    p->next = newnode;

    list->currentLen++;
    return;
}

int dequeue(Linklist *list){
    if (list->currentLen<=0){
        printf("Error: Dequeue\n");
        return -1;
    }
    Node *p=(list->head)->next;
    Node *q=p->next;
    (list->head)->next=q;
    int ret=p->data;
    free(p);
    list->currentLen--;
    return ret;
}

void destroy(Linklist *list){
    int i=list->currentLen;
    while(i-->0) dequeue(list);
    
    free(list->head);
}

//random delay
void delay()
{
	sleep(2 + rand()%3);
}

void shortdelay()
{
	sleep(1 + (double)rand()/RAND_MAX);
}

void longdelay()
{
	sleep(4 + rand()%6);
}

Linklist queue;
int num_imm =0, num_spe = 0, num_jud = 0, num_imm_uncheck = 0, num_imm_unconfirm = 0;

sem_t jud_in, imm_check, sitdown, unsitdown, imm_confirmed[MAX_THREAD];

void *spectator()
{
	sem_wait(&jud_in);	//cannot enter when judge is in
	sem_post(&jud_in);
	int spec_no = num_spe++;
	shortdelay();
	printf("Spectator #%d enter\n", spec_no);
	shortdelay();
	printf("Spectator #%d spectate\n", spec_no);
	longdelay();
	printf("Spectator #%d leave\n", spec_no);
	num_cur_thread--;
	pthread_exit(NULL);
}

void *immigrant()
{
    sem_wait(&jud_in);	//cannot enter when judge is in
	sem_post(&jud_in);
	int imm_no = num_imm++;	//update the index of immigrants
    num_imm_uncheck++;
	num_imm_unconfirm++;
	shortdelay();
	printf("Immigrant #%d enter\n", imm_no);
	shortdelay();
	printf("Immigrant #%d checkIn\n", imm_no);
	sem_post(&imm_check);
	num_imm_uncheck--;
	sem_wait(&unsitdown);	//cannot sit down when there is no chairs.
	shortdelay();
	printf("Immigrant #%d sitDown\n", imm_no);
	enqueue(&queue, imm_no);	//enqueue the immigrant
	sem_post(&sitdown);
	sem_wait(&imm_confirmed[imm_no % MAX_THREAD]);
	shortdelay();
	printf("Immigrant #%d getCertificate\n", imm_no);
	sem_wait(&jud_in);	//cannot leave when judge is in
	shortdelay();
	printf("Immigrant #%d leave\n", imm_no);
	sem_post(&jud_in);
	num_cur_thread--;
	pthread_exit(NULL);
}

void *judge()
{
	sem_wait(&jud_in);	//cannot enter when there is already a judge in
	int jud_no = num_jud++;
	shortdelay();
	printf("Judge #%d enter\n", jud_no);

	int i;
	for (i = 0; i < num_imm_uncheck; ++i)
		sem_wait(&imm_check);	//Once all immigrants check in, the judge can confirm
	if (num_imm_unconfirm != 0)
	{
		do{
			sem_wait(&sitdown);
			int imm_no = dequeue(&queue);	//dequeue the immigrant
			shortdelay();
			printf("Judge #%d confirm the immigrant #%d \n", jud_no, imm_no);
			num_imm_unconfirm--;
			sem_post(&imm_confirmed[imm_no % MAX_THREAD]);
			sem_post(&unsitdown);
		}while(num_imm_unconfirm > 0);
	}
	shortdelay();
	printf("Judge #%d leave\n", jud_no);
	sem_post(&jud_in);
	num_cur_thread--;
	pthread_exit(NULL);
}

int index_thread = 0;
pthread_t *threads;

int new_thread(int index)
{
	if (num_cur_thread>MAX_THREAD) return 0;
	else if (num_cur_thread==MAX_THREAD){
		pthread_create(&threads[index % MAX_THREAD], NULL, judge, NULL);
		return 1;
	}
	int rnd = rand()%8;
	if (rnd<=3) pthread_create(&threads[index % MAX_THREAD], NULL, immigrant, NULL);
	else if (rnd<=5 && rnd>3) pthread_create(&threads[index % MAX_THREAD], NULL, spectator, NULL);
	else pthread_create(&threads[index % MAX_THREAD], NULL, judge, NULL);
	// switch(rand()%3)
	// {
	// 	case 0:pthread_create(&threads[index % MAX_THREAD], NULL, immigrant, NULL); break;
	// 	case 1:pthread_create(&threads[index % MAX_THREAD], NULL, spectator, NULL); break;
	// 	case 2:pthread_create(&threads[index % MAX_THREAD], NULL, judge, NULL); break;
	// }
	return 1;
}

int main(){
    srand(time(NULL));
	threads = malloc(MAX_THREAD *sizeof(pthread_t));
    init(&queue);
    sem_init(&jud_in, 0, 1);
	sem_init(&imm_check, 0, 0);
    sem_init(&sitdown, 0, 0);
	sem_init(&unsitdown, 0, MAX_CHAIR);
    for (int i = 0; i<MAX_THREAD; ++i)
		sem_init(&imm_confirmed[i], 0, 0);
    while(1)
	{
		num_cur_thread++;
		int ret = new_thread(index_thread);
		if (!ret) num_cur_thread--;
		else index_thread++;
		delay();
	}
    destroy(&queue);
}