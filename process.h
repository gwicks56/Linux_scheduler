/*
Name: Geordie Wicks
Username: gwicks
Student ID: 185828

COMP30023 Project 1

NOTE: List / Queue code canabalised from 2nd Year Design Of Algorithms Project
*/




#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <stdlib.h>

/*
define Process
*/
typedef struct _Process{
	int timeCreated;
	int processID;	
	int memorySize;
	int jobTime;

	/*time processed*/
	int processedTime;

	/*time when it was placed in memory*/
	int memoryTime;

	/*swapped out*/
	int swappedOut;

} Process;

Process* createProcess(int timeCreated,	int processID, int memorySize, int jobTime);

#endif