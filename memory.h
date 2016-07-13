/*
Name: Geordie Wicks
Username: gwicks
Student ID: 185828

COMP30023 Project 1

NOTE: List / Queue code canabalised from 2nd Year Design Of Algorithms Project
*/

#ifndef _MEMORY_H
#define _MEMORY_H

#include "process.h"

// queue node structure
typedef struct MEMORY_NODE{	
	//node data, null if free
	Process *process;
	int size;
	struct MEMORY_NODE *next;
} MemoryNode;

// queue structure
typedef struct MEMORY{
	MemoryNode* head;
	int totalSize;
} Memory;

/*
create memory
*/
Memory* createMemory(int memorySize);

/*
allocate memory
return 1 if OK, 0 for fail
*/
int allocateMemory(Memory* memory, Process* process);

/*
deallocate memory (release, merge continueous free slot)
*/
void deallocateMemory(Memory* memory, Process* process);

/*
swap out process that is largest size
if two are equal size, choose the one that has been in memory the longest
*/
Process* swapOutProcessInMemory(Memory* memory);

/*return total of used size*/
int getUsedSizeMemory(Memory* memory);

/*return count of free holes*/
int getFreeHolesMemory(Memory* memory);

/*return count of processes in memory*/
int getCountProcessesMemory(Memory* memory);

#endif