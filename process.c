/*
Name: Geordie Wicks
Username: gwicks
Student ID: 185828

COMP30023 Project 1

NOTE: List / Queue code canabalised from 2nd Year Design Of Algorithms Project
*/




#include "process.h"

Process* createProcess(int timeCreated,	int processID, int memorySize, int jobTime){
	
	Process* p = (Process*)malloc(sizeof(Process));
	p->timeCreated = timeCreated;
	p->processID = processID;
	p->memorySize = memorySize;
	p->jobTime = jobTime;

	p->processedTime = 0;
	p->memoryTime = -1; /*will be set later*/
	p->swappedOut = 0;
	return p;
}