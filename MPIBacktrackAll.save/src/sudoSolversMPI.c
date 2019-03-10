// Copyright (c) 2019 Jason Graalum,
//
// CS 531 Performance Analysis
// Portland State University
// Winter 2019
//
// Jason Graalum
// Feb 2, 2019
//

#include "sudoSolvers.h"
#include "sudoSolvUtils.h"
#include <sys/queue.h>
#include <mpi.h>

int btSolve(int degree, Cell * s)
{
    return(addGuess(degree, s, 0, 0));
}

int addGuess(int degree, Cell * s, int row, int col)
{
    int pos = row*degree + col;
    // If cell already has a value
    if(s[pos] != 0) {
        int new_row, new_col;
        new_row = (row + 1) % degree;
        new_col = new_row == 0 ? (col + 1) : col;

        // If row and col are the last cell, we have a solution
        if(new_col == degree) return(verifySolution(degree,s));

        // Call addGuess. If it returns greater than 0, we have  a solution
        // If it returns less than 0, this guess_val isn't valid
        return(addGuess(degree, s, new_row, new_col) > 0); 
    }

    Cell guess_val = 1;
    do {
        // Try a number
        int pos = row*degree + col;
        s[pos] = guess_val;

        // If valid, increment cell indexes and call addGuess
        if(verifySolution(degree,s) > 0) {
            int new_row, new_col;
            new_row = (row + 1) % degree;
            new_col = new_row == 0 ? (col + 1) : col;

            // If row and col are the last cell, w e have a solution
            if(new_col == degree) return(1);

            // Call addGuess. If it returns greater than 0, we have a solution
            // If it returns less than 0, this guess_val isn't valid
            if(addGuess(degree, s, new_row, new_col) > 0) 
                return(1);
        }
        guess_val++;
        if(guess_val > degree) { 
            pos = row*degree + col;
            s[pos] = 0;
            return(-1);
        }
    } while(1);
}

// The solver waits for a Send from proc 0.
// There is a start message, a run message and a end message
int btAllSolver(int procId)
{
    // Wait for start
    int degree;

    int * retval = (int *)malloc(sizeof(int));
    // Recieve first size
    MPI_Recv(&degree, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    while(degree > 0) {
        Cell * solution = (Cell *)malloc(sizeof(Cell)*degree*degree);
        // Recieve puzzle cell data
        MPI_Recv(&solution, degree*degree, mpiCell, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        *retval = btSolve(msgSize,solution);
        free(solution);
        
        // Send results - 0 fail, 1 pass
        MPI_Send(retval, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        // Recieve next size
        MPI_Recv(&degree, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    return(0);
}





