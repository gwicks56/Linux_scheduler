/*
Name: Geordie Wicks
Username: gwicks
Student ID: 185828

COMP30023 Project 1

NOTE: List / Queue code canabalised from 2nd Year Design Of Algorithms Project
*/





#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "queue.h"
#include "memory.h"

/*
length of each queue level
*/
#define QUANTUM_QUEUE_1 2
#define QUANTUM_QUEUE_2 4
#define QUANTUM_QUEUE_3 20

/*
read processes from file to dispatch list
*/
void readProcesses(Queue *dispatchList, char* inputFileName){
	/*timeCreated, processID, memorySize, memoryRequested, jobTime*/
	FILE * fp;

	/*a line for a process*/
	int timeCreated;
	int processID;	
	int memorySize;
	int jobTime;

	fp = fopen(inputFileName, "r");
    if (fp != NULL){
        /*read line by line*/
		while (!feof(fp)){
			if (fscanf(fp, "%d %d %d %d\n", &timeCreated, &processID, &memorySize, &jobTime) == 4){
				enqueueQueue(dispatchList, createProcess(timeCreated, processID, memorySize, jobTime));
			}
		}
	}else{
		printf("Cannot open input file for reading\n");
	}
}
/*
first come, first serve algorithm
*/
void fcfs(Queue* dispatchList, Memory* memory){
	Process* process;
	int usedSize; 			/*using size in memory*/
	int simulationTime = 0; /*simulation time*/
	int freeHoles; 			/*number of free holes*/
	int numProcesses; 		/*number of processes in memory*/
	int processCount = 0;
	int jobFinishTimeCount = 0;

	/*get process one by one and run until it finishes*/
	while (isEmptyQueue(dispatchList) == 0)
	{
		processCount++;
		process = dequeueQueue(dispatchList);
		
		/*process until finishing*/
		allocateMemory(memory, process);

		usedSize = getUsedSizeMemory(memory);
		freeHoles = getFreeHolesMemory(memory);
		numProcesses = getCountProcessesMemory(memory);

		printf("time %d, %d running, numprocesses=%d, numholes=%d, memusage=%d%%\n", 
			simulationTime, process->processID, numProcesses, freeHoles, usedSize * 100 / memory->totalSize);

		/*move simulation*/
		simulationTime += process->jobTime;
		jobFinishTimeCount += simulationTime;

		/*deallocate and free process*/
		deallocateMemory(memory, process);
		free (process);
	}
	printf("time %d, simulation finished.\n", simulationTime);
	//printf("the average job finish time is: %d", jobFinishTimeCount / processCount);
}

/*
multi queue feedback algorithm
processes are run in queue 1, then if still running
moved to queue 2, then if still running
moved to queue 3
*/
void multi(Queue* dispatchList, Memory* memory, Queue *firstQueue, Queue *secondQueue, Queue *thirdQueue){
	int processed; 											/*if any queue executes*/
	Process* process, *swapOutProcess;
	int usedSize; 											/*used size in memory*/
	int simulationTime = 0; 								/*simulation time initialised to zero*/
	int freeHoles; 											/*number of free holes*/
	int numProcesses;
	int totalProcesses = 0;
	int finishTimeCount = 0; 										/*number of processes in memory*/

	/*choose process and execute*/
	while (isEmptyQueue(dispatchList) == 0 || isEmptyQueue(firstQueue) == 0 || isEmptyQueue(secondQueue) == 0 || 
		isEmptyQueue(thirdQueue) == 0)
	{
		processed = 0; /*not yet execute*/		

		/*step 1 - load from dispatch list to first queue if any*/
		if (isEmptyQueue(dispatchList) == 0)
		{
			process = peekQueue(dispatchList);
			if (process->timeCreated <= simulationTime)
			{
				process = dequeueQueue(dispatchList);
				
				/*allocate*/
				while (allocateMemory(memory, process) == 0)
				{
					swapOutProcess = swapOutProcessInMemory(memory);
					swapOutProcess->swappedOut = 1;
				}
				/*allocate successfully*/
				process->memoryTime = simulationTime;

				/*push to first queque*/
				enqueueQueue(firstQueue, process);
			}else if (isEmptyQueue(firstQueue) == 1 && isEmptyQueue(secondQueue) == 1 && 
					isEmptyQueue(thirdQueue) == 1){/*all queues are empty, increase simulation time and move process to first queue*/

				process = dequeueQueue(dispatchList);
				simulationTime = process->timeCreated;
				allocateMemory(memory, process);

				/*allocate successfully*/
				process->memoryTime = simulationTime;

				/*push to first queque*/
				enqueueQueue(firstQueue, process);
			}
		}		

		/*step 2 - process first queue if any*/
		if (isEmptyQueue(firstQueue) == 0)
		{
			processed = 1; /*run process in first queue*/
			process = dequeueQueue(firstQueue);
			if (process->swappedOut == 1)
			{
				/*allocate*/
				while (allocateMemory(memory, process) == 0)
				{
					swapOutProcess = swapOutProcessInMemory(memory);
					swapOutProcess->swappedOut = 1;
				}
				/*allocate successfully*/
				process->memoryTime = simulationTime;
				process->swappedOut = 0;
			}
			usedSize = getUsedSizeMemory(memory);
			freeHoles = getFreeHolesMemory(memory);
			numProcesses = getCountProcessesMemory(memory);

			printf("time %d, %d running, numprocesses=%d, numholes=%d, memusage=%d%%\n", 
				simulationTime, process->processID, numProcesses, freeHoles, usedSize * 100 / memory->totalSize);

			/*move simulation*/
			if (process->processedTime + QUANTUM_QUEUE_1 >= process->jobTime)
			{
				simulationTime += process->jobTime - process->processedTime;
				//printf("Job finished from first queue at: %d\n", simulationTime);
				finishTimeCount += simulationTime;
				totalProcesses++;

				/*deallocate and free process*/
				deallocateMemory(memory, process);
				free (process);
			}else{
				simulationTime += QUANTUM_QUEUE_1;
				process->processedTime += QUANTUM_QUEUE_1;
				enqueueQueue(secondQueue, process);
			}
		}

		/*step 3 - process second queue  if any*/
		if (processed == 0 && isEmptyQueue(secondQueue) == 0)
		{
			processed = 1; /*run process in second queue*/
			process = dequeueQueue(secondQueue);
			if (process->swappedOut == 1)
			{
				/*allocate*/
				while (allocateMemory(memory, process) == 0)
				{
					swapOutProcess = swapOutProcessInMemory(memory);
					swapOutProcess->swappedOut = 1;
				}
				/*allocate successfully*/
				process->memoryTime = simulationTime;
				process->swappedOut = 0;
			}

			usedSize = getUsedSizeMemory(memory);
			freeHoles = getFreeHolesMemory(memory);
			numProcesses = getCountProcessesMemory(memory);

			printf("time %d, %d running, numprocesses=%d, numholes=%d, memusage=%d%%\n", 
				simulationTime, process->processID, numProcesses, freeHoles, usedSize * 100 / memory->totalSize);

			/*move simulation*/
			if (process->processedTime + QUANTUM_QUEUE_2 >= process->jobTime)
			{
				simulationTime += process->jobTime - process->processedTime;
				//printf("Job finished during second queue at: %d\n", simulationTime);
				finishTimeCount += simulationTime;
				totalProcesses++;

				/*deallocate and free process*/
				deallocateMemory(memory, process);
				free (process);
			}else{
				simulationTime += QUANTUM_QUEUE_2;
				process->processedTime += QUANTUM_QUEUE_2;
				enqueueQueue(thirdQueue, process);
			}
		}

		/*step 4 - process third queue  if any*/
		if (processed == 0 && isEmptyQueue(thirdQueue) == 0)
		{
			processed = 1; /*run process in third queue*/
			process = dequeueQueue(thirdQueue);
			if (process->swappedOut == 1)
			{
				/*allocate*/
				while (allocateMemory(memory, process) == 0)
				{
					swapOutProcess = swapOutProcessInMemory(memory);
					swapOutProcess->swappedOut = 1;
				}
				/*allocate successfully*/
				process->memoryTime = simulationTime;
				process->swappedOut = 0;
			}

			usedSize = getUsedSizeMemory(memory);
			freeHoles = getFreeHolesMemory(memory);
			numProcesses = getCountProcessesMemory(memory);

			printf("time %d, %d running, numprocesses=%d, numholes=%d, memusage=%d%%\n", 
				simulationTime, process->processID, numProcesses, freeHoles, usedSize * 100 / memory->totalSize);

			/*move simulation*/
			if (process->processedTime + QUANTUM_QUEUE_3 >= process->jobTime)
			{
				simulationTime += process->jobTime - process->processedTime;
				//printf("Job ID: %d finished from third queue at: %d\n",process->processID, simulationTime);
				finishTimeCount += simulationTime;
				totalProcesses++;

				/*deallocate and free process*/
				deallocateMemory(memory, process);
				free (process);
			}else{
				simulationTime += QUANTUM_QUEUE_3;
				process->processedTime += QUANTUM_QUEUE_3;
				enqueueQueue(thirdQueue, process);
			}
		}
	}
	printf("time %d, simulation finished.\n", simulationTime);
	//printf("the average finish time is: %d\n", finishTimeCount / totalProcesses);
}

//sample:
//./simulation -f in.txt -a fcfs -m 200 > out.txt
int main ( int argc, char *argv[]){
	Queue *dispatchList; 									/*processes*/
	Queue *firstQueue, *secondQueue, *thirdQueue;

	Memory *memory; 										/*memory*/
	char* filename; 										/*file name*/
	char* algorithm; 										/*algorithm is fcfs or multi*/
	int memorySize = 0; 									/*memory size*/
	int i; 													/*loop control variable*/

	if (argc != 7)
	{
		printf("Command line example: %s -f <input file> -a [fcfs|multi] -m <memory size>\n", argv[0]);
		return 0;
	}

	for (i = 1; i < argc - 1; i+=2)
	{
		if (strcmp(argv[i], "-f") == 0)
		{
			filename = argv[i + 1];
		}else if (strcmp(argv[i], "-a") == 0)
		{
			algorithm = argv[i + 1];
		}else if (strcmp(argv[i], "-m") == 0)
		{
			memorySize = atoi(argv[i + 1]);
		}
	}

	dispatchList = createQueue();
	memory = createMemory(memorySize);

	/*read process file file*/
	readProcesses(dispatchList, filename);

	/*simulate*/
	if (strcmp(algorithm, "fcfs") == 0)
	{
		fcfs(dispatchList, memory);
	}else{
		/*create 3 priority queues*/
		firstQueue = createQueue();
		secondQueue = createQueue();
		thirdQueue = createQueue();

		multi(dispatchList, memory, firstQueue, secondQueue, thirdQueue);

		/*free resources*/
		free (firstQueue);
		free (secondQueue);
		free (thirdQueue);
	}

	/*free resources*/
	free (memory);
	free (dispatchList);

	return 0;
}