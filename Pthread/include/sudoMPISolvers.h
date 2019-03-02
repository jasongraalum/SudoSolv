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

#ifndef _SUDOMPISolver_H_
#define _SUDOMPISolver_H_
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "sudoMPISolvUtils.h"

int treeMPISolve(Puzzle *);
int addMPIGuess(Puzzle *, int, int);

#endif


