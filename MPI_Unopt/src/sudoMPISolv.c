// Copyright (c) 2019 Jason Graalum,
//
// CS 531 Performance Analysis
// Portland State University
// Winter 2019
//
// Jason Graalum
// Feb 2, 2019
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

#include <sudoMPISolvUtils.h>
#include <sudoMPISolvers.h>

int main(int argc, char **argv)
{
    int numProcs, procId;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    Puzzle *p;

    char *inFilename;
    char *solver;
    int repeats=1;

    // mpiPuzzle Datatype
    int puzzleBlocksCount = 4;
    int puzzleBlocksLength[4] = {1,1,1,1};
    MPI_Datatype puzzleTypes[4] = {MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED, MPI_UNSIGNED};
    MPI_Aint puzzleOffsets[4];
    MPI_Datatype mpiPuzzle;
    puzzleOffsets[0] = offsetof(Puzzle, degree);
    puzzleOffsets[1] = offsetof(Puzzle, setCount);
    puzzleOffsets[2] = offsetof(Puzzle, getCount);
    puzzleOffsets[3] = offsetof(Puzzle, cell);
    MPI_Type_create_struct(puzzleBlocksCount, puzzleBlocksLength, puzzleOffsets, puzzleTypes ,&mpiPuzzle);
    MPI_Type_commit(&mpiPuzzle);

    // mpiCell Datatype
    MPI_Datatype mpiCell;
    MPI_Type_create_resized(MPI_UNSIGNED, 0, sizeof(Cell), &mpiCell);
    MPI_Type_commit(&mpiCell);

    struct timespec ts_totaltime_start;
    struct timespec ts_totaltime_end;

    struct timespec ts_realtime_start;
    struct timespec ts_realtime_end;

    clock_gettime(CLOCK_REALTIME, &ts_totaltime_start);
    //double *time_data;
    //int *results;
    //int *sets;
    //int *gets;

    if(procId == 0) {
        if(argc != 4)
        {
            printf("Usage: sudoSolv <solver> <n repeats> <starting puzzle>\n");
            exit(-1);
        }

        solver = argv[1]; //(char*)malloc(sizeof(char)*(strlen(argv[1])+1));
        //strncpy(solver,argv[1],strlen(argv[1]));
        repeats = atoi(argv[2]);
        inFilename = argv[3]; //(char*)malloc(sizeof(char)*(strlen(argv[3])+1));
        //strncpy(inFilename,argv[3],strlen(argv[3]));

        printf("Solver: %s\n", solver);
        printf("Repeats: %d\n", repeats);
        printf("Input file: %s\n", inFilename);
    }

    for(int repeat = 0; repeat < repeats; repeat++) {
        /*
           time_data = (double *)malloc(sizeof(double)*repeats);
           results = (int *)malloc(sizeof(int)*repeats);
           sets = (int *)malloc(sizeof(int)*repeats); 
           gets = (int *)malloc(sizeof(int)*repeats);
           */

        if(procId == 0) {
            // Setup and print starting puzzle
            p = loadPuzzle(inFilename);
            if (p == NULL) {
                printf("Error loading puzzle. Exitting.\n");
            }
            printf("ProcId = %d\n", procId);
            printPuzzle(p);
        }
        else
        {
            p = (Puzzle *)malloc(sizeof(Puzzle));
            p->degree = 0;
            p->cell = NULL;
            p->setCount = 0;
            p->getCount = 0;
        }

        clock_gettime(CLOCK_REALTIME, &ts_realtime_start);
        //
        // Tranfer Puzzle to all other processes
        //
        MPI_Bcast(p, 1, mpiPuzzle, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);

        // Transmit puzzle cell contents
        int cell_count = p->degree*p->degree;
        if(procId != 0) p->cell = (Cell *)malloc(sizeof(Cell)*cell_count);
        MPI_Bcast(p->cell,cell_count, mpiCell, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);

        int result = treeMPISolve(p, 0, 0, numProcs, procId);


        if(procId == 0) {
            if(result > 0) {
                printf("Puzzle Solved\n");
                printPuzzle(p);
            }
            else {
                printf("Unable to solve puzzle.\n");
                printPuzzle(p);
            }
        }
        clock_gettime(CLOCK_REALTIME, &ts_realtime_end);
        free(p->cell);
        free(p);
    }
    clock_gettime(CLOCK_REALTIME, &ts_totaltime_end);


    int totalSetCount;
    MPI_Reduce((void *)&p->setCount,(void*)&totalSetCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    int totalGetCount;
    MPI_Reduce((void*)&p->getCount,(void*)&totalGetCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if(procId == 0) {
        printf("Total sets = %d\n", totalSetCount);
        printf("Total gets = %d\n", totalGetCount);
    }
    MPI_Finalize();
    return 0;
}

