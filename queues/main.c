#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "queue.h"

queue_t *queue;

void *addToQ(void *a);
void *removeFromQ(void *a);

int main(int argc, char *argv[]) 
{
    queue = malloc(sizeof(queue_t));
    int numToQ = 5;
    Queue_Init(queue);

    pthread_t t1, t2;

    pthread_create(&t1, NULL, addToQ, (void *)numToQ);
    pthread_create(&t2, NULL, removeFromQ, (void *)numToQ);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}

void *addToQ(void *a)
{
    int arg = (int) a;
    Enqueue(queue, arg);

    printf("Add:\n");       
    printf("Head: %d\n", queue->head->value);
    printf("Tail: %d\n", queue->tail->value);
    return NULL;
}

void *removeFromQ(void *a)
{
    
    int arg = (int) a;
    Dequeue(queue, &arg);

    printf("Remove:\n");     
    printf("Head: %d\n", queue->head->value);
    printf("Tail: %d\n", queue->tail->value);
    return NULL;
}
