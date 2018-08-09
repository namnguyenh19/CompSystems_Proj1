#
# Sample Makefile for project 1
#
# The program used for illustration purposes is
# a simple program using prime numbers
#


## CC  = Compiler.
## CFLAGS = Compiler flags.
CC	= gcc
CFLAGS =	-Wall -Wextra -std=c99 -g

## HDR = Header files.
## OBJ = Object files.
## SRC = Source files.
## EXE = Executable name.

HDR = main_header.h
SRC =		driver.c list.c memory.c scheduler.c
OBJ =		$(SRC:.c=.o)
EXE = 		swap

## Top level target is executable.
$(EXE):	$(OBJ)
		$(CC) $(CFLAGS) -o $(EXE) $(OBJ) -lm


## Clean: Remove object files and core dump files.
clean:
		/bin/rm $(OBJ)

## Clobber: Performs Clean and removes executable file.

clobber: clean
		/bin/rm $(EXE)

## Dependencies

$(OBJ) : $(HDR)
