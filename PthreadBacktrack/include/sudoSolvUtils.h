// // Copyright (c) 2019 Jason Graalum,
//
// CS 531 Performance Analysis
// Portland State University
// Winter 2019
//
// Jason Graalum
// Feb 2, 2019
//
// sudoSolv Data structure
//
// Puzzle Data Structure

#ifndef _SUDOStruct_H_
#define _SUDOStruct_H_
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// Each cell can have degree + 1 values - the +1 is for unknown.
// To simplify - will optimize memory later
typedef long int Cell;

// Replace cell with int for now - optimize memory later
typedef struct Puzzle {
        int degree;
        int setCount;
        int getCount;
	Cell *cell;
} Puzzle;

Puzzle *p;

size_t xlatRowCol(int, int, int);
int setCell(int, int, int);
int getCell(int, int);

int loadPuzzle(char *);

int printPuzzle();

int getBlockRow(int, int);
int getBlockCol(int, int);

int checkPuzzleRow(int);
int checkPuzzleCol(int);
int checkPuzzleBlock(int, int);

void * checkPuzzleRows();
void * checkPuzzleCols();
void * checkPuzzleBlocks();

int verifyPuzzle();

#endif
