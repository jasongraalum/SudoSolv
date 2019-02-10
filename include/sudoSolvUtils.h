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

// Set the puzzle degree - maybe define during compile time
#ifndef PDEGREE
  #define PDEGREE 9
#endif

// Each cell can have PDEGREE + 1 values - the +1 is for unknown.
// To simplify - will optimize memory later
typedef int Cell;

// Replace cell with int for now - optimize memory later
typedef struct Puzzle {
	Cell cell[PDEGREE * PDEGREE];
} Puzzle;

int setCell(Puzzle *, int, int, Cell);
Cell getCell(Puzzle *, int, int);
int loadPuzzle(char *, Puzzle *);
int printPuzzle(Puzzle *);
int initPuzzle(Puzzle *);
int verifyPuzzle(Puzzle *);

#endif
