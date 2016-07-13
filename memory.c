/*
Name: Geordie Wicks
Username: gwicks
Student ID: 185828

COMP30023 Project 1

NOTE: List / Queue code canabalised from 2nd Year Design Of Algorithms Project
*/






#include "memory.h"

int memorySwaps = 0;

/*
create memory
return pointer to memory
*/
Memory* createMemory(int memorySize){
	/*create memory*/
	MemoryNode* node;
	Memory* memory = (Memory*)malloc(sizeof(Memory));
	memory->totalSize = memorySize;

	/*create unique node*/
	node = (MemoryNode*)malloc(sizeof(MemoryNode));
	node->process = NULL;
	node->next = NULL;
	node->size = memorySize;
	memory->head = node;
	return memory;
}

/*
allocate memory
*/
int allocateMemory(Memory* memory, Process* process){
	MemoryNode* node;
	MemoryNode* current = memory->head;

	/*iterate the linked list*/
	while (current != NULL)
	{
		//check free
		if (current->process == NULL && current->size >= process->memorySize)
		{
			/*maybe split?*/
			if (current->size == process->memorySize)
			{
				current->process = process;
			}else{
				/*create new smaller size node*/
				node = (MemoryNode*)malloc(sizeof(MemoryNode));
				node->process = NULL;
				node->size = current->size - process->memorySize;
				node->next = current->next;				

				/*edit current node*/
				current->next = node;
				current->size = process->memorySize;
				current->process = process;
			}
			return 1;
		}
		current = current->next;
	}
	return 0;
}

/*
deallocate memory (release, merge continueous free slot)
*/
void deallocateMemory(Memory* memory, Process* process){

	MemoryNode* current = memory->head;
	MemoryNode* temp;
	while (current != NULL)
	{
		if (current->process == process)
		{
			current->process = NULL;
			break;
		}
		current = current->next;
	}
	
	current = memory->head;

	/*iterate the linked list*/
	while (current != NULL){
		/*two continuous free slots*/
		if (current->process == NULL && current->next != NULL && current->next->process == NULL)
		{
			temp = current->next;
			current->size += temp->size;
			current->next = temp->next;
			free (temp);
		}else{
			current = current->next;
		}
	}
}

/*
swap out process that is largest size
if two are equal size, choose the one 
that has been in memory the longest
*/
Process* swapOutProcessInMemory(Memory* memory){
	Process* p = NULL;
	MemoryNode* node = memory->head;

	/*iterate the linked list*/
	while (node != NULL)
	{
		if (node->process != NULL){
			if (p == NULL ||
			(node->process->memorySize > p->memorySize ||
			(node->process->memorySize == p->memorySize && node->process->memoryTime < p->memoryTime)))
			{
				p = node->process;
			}
		}
		node = node->next;
	}

	/*remove this process from memory (and merge free slots)*/
	deallocateMemory(memory, p);
	//printf("memory removed from main\n");
	memorySwaps++;
	//printf("total memory swaps to date: %d\n", memorySwaps);
	return p;
}

/*return total of used size*/
int getUsedSizeMemory(Memory* memory){
	
	int totalUsed = 0;

	MemoryNode* current = memory->head;

	/*iterate the linked list*/
	while (current != NULL)
	{
		if (current->process != NULL)
		{
			totalUsed += current->size;
		}
		current = current->next;
	}

	return totalUsed;
}

/*return count of free holes*/
int getFreeHolesMemory(Memory* memory){
	int totalFree = 0;

	MemoryNode* current = memory->head;

	/*iterate the linked list*/
	while (current != NULL)
	{
		if (current->process == NULL)
		{
			totalFree ++;
		}
		current = current->next;
	}
	return totalFree;
}

/*return count of processes in memory*/
int getCountProcessesMemory(Memory* memory){
	int numProcess = 0;

	MemoryNode* current = memory->head;

	/*iterate the linked list*/
	while (current != NULL)
	{
		if (current->process != NULL)
		{
			numProcess ++;
		}
		current = current->next;
	}

	return numProcess;
}