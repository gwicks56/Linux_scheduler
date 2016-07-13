/*
Name: Geordie Wicks
Username: gwicks
Student ID: 185828

COMP30023 Project 1

NOTE: List / Queue code canabalised from 2nd Year Design Of Algorithms Project
*/




#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdio.h>
#include "process.h"

// queue node structure
typedef struct QUEUE_NODE{	
	//node data
	Process *process;
	struct QUEUE_NODE *next;
} QueueNode;

// queue structure
typedef struct QUEUE{
	QueueNode* front;
} Queue;

/*
create queue
*/
Queue* createQueue();

/*
check if queue is empty
return 1 for empty and 0 for not empty
*/
int isEmptyQueue(Queue *queue);

/*
place process at the tail end of Queue
*/
void enqueueQueue(Queue *queue, Process* process);

/*
remove process from front of queue
*/
Process* dequeueQueue(Queue *queue);

/*
retrieve but not remove from front of queue
*/
Process* peekQueue(Queue *queue);

#endif