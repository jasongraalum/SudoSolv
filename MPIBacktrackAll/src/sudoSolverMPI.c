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

        // Call addGuess. If it returns greater than 0, we have a solution
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

            // If row and col are the last cell, we have a solution
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


int mpiSolver(int procId) 
{
    int degree;
    int *result = (int *)malloc(sizeof(int));
    *result = 0;
    Cell * soln;
    MPI_Status status;
    do {
        //printf("Waiting for rec:%d\n",procId); 
        MPI_Recv(&degree, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("Received degree: %d\n", degree); 
        if(degree > 0) {
            //printf("Waiting for solution to try\n");
            soln = (Cell *)malloc(sizeof(Cell) * degree * degree);
            MPI_Recv(soln, degree*degree, mpiCell, 0, 0, MPI_COMM_WORLD, &status);
            //printf("Received solution to try\n");
            //printf("soln[0] = %d\n",soln[0]);
            *result = btSolve(degree, soln);
            printf("Result(%d): %d\n", procId, *result);
            MPI_Send(result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            if(*result == 1)
                MPI_Send(soln, degree*degree, mpiCell, 0, 0, MPI_COMM_WORLD);
        }
    }
    while(degree != -1);

    return(*result);
}
