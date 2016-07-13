
##	Name: Geordie Wicks
##	Username: gwicks
##	Student ID: 185828

##	COMP30023 Project 1
##  With thanks to whomever supplied example on LMS


## CC  = Compiler.
## CFLAGS = Compiler flags.
CC	= gcc
CFLAGS 	= -Wall  


## OBJ = Object files.
## SRC = Source files.
## EXE = Executable name.

SRC =		memory.c process.c queue.c
OBJ =		memory.o process.o queue.o simulation.o
EXE = 		simulation

simulation:   $(OBJ) Makefile
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ)



clean:
		rm -f $(OBJ) $(EXE)



## Dependencies

memory.o: memory.h memory.c process.h
process.o: process.h process.c
queue.o: queue.h queue.c process.h
simulation: process.h queue.h memory.h simulation.c
