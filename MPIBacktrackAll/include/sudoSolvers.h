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

#ifndef _SUDOSolver_H_
#define _SUDOSolver_H_
#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>

#include "sudoSolvUtils.h"

typedef struct trial {
    int pos;
    int index;
    int degree;
    SolNode * sn;
    Cell * solution;
} trial_t;

typedef struct procEntry {
    int procId;
    int numProcs;
    trial_t trial;
    TAILQ_ENTRY(procEntry) entries;
} procEntry_t;

typedef TAILQ_HEAD(procFreeQ_head, procEntry) procFreeQ_head_t;

typedef struct solEntry {
    trial_t * trial;
    int procId;
    TAILQ_ENTRY(solEntry) entries;
} solEntry_t;


typedef TAILQ_HEAD(solQ_head, solEntry) solQ_head_t;


int btAllSolver(Puzzle *, int);
int btSolve(int, Cell *);
trial_t * getNextSolution(Puzzle *, SolNode *, Cell *, int);
//int buildNextSolution(Puzzle *, SolNode *, Cell *, int, procFreeQ_head_t *);
int addGuess(int, Cell *, int, int);
int mpiSolver(int);

MPI_Datatype mpiCell;
#endif


