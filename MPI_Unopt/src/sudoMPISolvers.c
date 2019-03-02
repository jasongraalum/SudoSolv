// Copyright (c) 2019 Jason Graalum,
//
// CS 531 Performance Analysis
// Portland State University
// Winter 2019
//
// Jason Graalum
// Feb 2, 2019
//

#include "sudoMPISolvers.h"
#include "sudoMPISolvUtils.h"

int treeMPISolve(Puzzle * p, int row, int col, int numProcs, int procId)
{
    int new_x, new_y;
    int * proc_res = (int*)malloc(sizeof(int));
    int *all_results = (int*)malloc(sizeof(int)*numProcs);

    int length = -1;
    char myHostName[MPI_MAX_PROCESSOR_NAME];
    MPI_Get_processor_name(myHostName,&length);
    //printf("%d - %s : entered treeSolver\n",procId, myHostName);
    // Get next empty cell, if no empty cell, return pass
    while(row < p->degree && col < p->degree && getCell(p, row, col) != 0) {
        row = (row + 1) % p->degree;
        col = (row == 0) ? (col + 1) : col;
    }
    if (col == p->degree) return(1);

    //printf("%d - %s: trying cell (%d,%d)\n", procId, myHostName, row, col);
    //
    // setCell and Verify
    //
    setCell(p, row, col, procId+1);
    *proc_res = verifyPuzzle(p);
    setCell(p, row, col, 0);
    //printf("%d - %s: verify returned %d\n", procId, myHostName, *proc_res);

    // Gather all results
    MPI_Allgather(proc_res,1,MPI_INT,all_results,1,MPI_INT, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if(procId == 0) {
    //for(int i = 0; i < numProcs; i++)
    //    printf("%d_",all_results[i]);
    //printf("\n");
    }

    // Pick first passing result
    for(int i = 0; i < numProcs; i++)
    {
        // setCell
        if(all_results[i] > 0) {
            setCell(p, row, col, i+1);
            if(procId == 0)
            {
                //printf("%d - %s: trying %d at (%d %d)\n",procId, myHostName, i+1, row, col);
           //     printPuzzle(p);
            }
            int next_proc_res = treeMPISolve(p, row, col, numProcs, procId);

            // If solver return is pass, return pass
            if(next_proc_res > 0) return(1);
            setCell(p, row, col, 0);
        }
        // If solver return is fail, try next passing result
    }
    return(0);
}

