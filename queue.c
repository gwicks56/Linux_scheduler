/*
Name: Geordie Wicks
Username: gwicks
Student ID: 185828

COMP30023 Project 1

NOTE: List / Queue code canabalised from 2nd Year Design Of Algorithms Project

NOTE: Helper printf code for monitoring
*/




#include "queue.h"

/*
create queue
*/
Queue* createQueue(){
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	queue->front = NULL;
	return queue;
}

/*
check if queue is empty
return 1 for empty and 0 for not empty
*/
int isEmptyQueue(Queue *queue){
	if (queue->front == NULL)
	{
		return 1;
	}else{
		return 0;
	}
}

/*
place process at the tail end of Queue
*/
void enqueueQueue(Queue *queue, Process* process){
	QueueNode* current; /*iterate queue to move to tail*/
	QueueNode* node = (QueueNode*)malloc(sizeof(QueueNode));
	node->process = process;
	node->next = NULL;

	if (isEmptyQueue(queue))
	{
		queue->front = node;
	}else{
		current = queue->front;
		/*move to tail*/
		while (current->next != NULL)
		{
			current = current->next;
		}
		/*link to new node*/
		current->next = node;
	}
}

/*
remove process from front of queue
*/
Process* dequeueQueue(Queue *queue){
	Process* process = queue->front->process;
	queue->front = queue->front->next;
	return process;
}

/*
retrieve but not remove from front of queue
*/
Process* peekQueue(Queue *queue){
	return queue->front->process;
}
