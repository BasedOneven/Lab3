#include <stdio.h>
#include <pthread.h>
#include "queue.h"

queue_t* queue;

int main(int argc, char *argv[]) 
{
    Queue_Init(queue);
    Enqueue(queue, 5);
}