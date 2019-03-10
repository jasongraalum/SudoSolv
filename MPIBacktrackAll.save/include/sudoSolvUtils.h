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
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Each cell can have degree + 1 values - the +1 is for unknown.
// To simplify - will optimize memory later
typedef char Cell;

typedef struct SolNode {
    int row;
    int col;
    struct SolNode ** next_cells;
} SolNode;

// Replace cell with int for now - optimize memory later
typedef struct Puzzle {
    int degree;
    int zeroLevel;
    int setCount;
    int getCount;
    int solCnt;
    struct SolNode * dead_end;
    struct SolNode * solution_head;
    Cell * fixed_cells;
} Puzzle;

size_t xlatRowCol(int, int, int);
int setCell(Puzzle *, int, int, Cell);
Cell getCell(Puzzle *, int, int);

Puzzle * loadPuzzle(char *);

int printPuzzle(Puzzle *);
int printSolution(Puzzle *, Cell *);

int getBlockRow(int, int);
int getBlockCol(int, int);

int checkPuzzleRow(int, Cell *, int);
int checkPuzzleCol(int, Cell *, int);
int checkPuzzleBlock(int, Cell *, int, int);

int verifySolution(int, Cell *);

SolNode * createNode(Puzzle *, int);
SolNode * addNodes(Puzzle *, Cell *, SolNode *, int);
int loadSolution(Puzzle *, Cell *);
int loadStartSolution(Puzzle *, Cell *);
int exploreSolution(Puzzle *, int);
void cleanUpTree(Puzzle *, SolNode *);
SolNode * copyZeroNodePath(Puzzle *, SolNode *);

MPI_Datatype mpiCell;
#endif
