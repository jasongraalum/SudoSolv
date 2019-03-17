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
    struct timespec ts_totaltime_start;
    struct timespec ts_totaltime_end;

    struct timespec ts_realtime_start;
    struct timespec ts_realtime_end;

    clock_gettime(CLOCK_REALTIME, &ts_totaltime_start);

    double *time_data;
    int *results;
    int *sets;
    int *gets;

    int numProcs, procId;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    Puzzle *p;

    char *inFilename;
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

    if(procId == 0) {
        if(argc != 3)
        {
            printf("Usage: sudoSolv <n repeats> <starting puzzle>\n");
            exit(-1);
        }

        repeats = atoi(argv[1]);
        inFilename = argv[2];

        printf("Repeats: %d\n", repeats);
        printf("Input file: %s\n", inFilename);

        time_data = (double *)malloc(sizeof(double)*repeats);
    }
    results = (int *)malloc(sizeof(int)*repeats);
    sets = (int *)malloc(sizeof(int)*repeats); 
    gets = (int *)malloc(sizeof(int)*repeats);

    if(procId != 0)  
    {
        p = (Puzzle *)malloc(sizeof(Puzzle));
        p->degree = 0;
        p->cell = NULL;
        p->setCount = 0;
        p->getCount = 0;
    }

    for(int repeat = 0; repeat < repeats; repeat++) {

        if(procId == 0) {
            // Setup and print starting puzzle
            p = loadPuzzle(inFilename);
            if (p == NULL) {
                printf("Error loading puzzle. Exitting.\n");
            }
            //printPuzzle(p);
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

        results[repeat] = btMPISolve(p, 0, 0, numProcs, procId);

        if(procId == 0)
        {
            clock_gettime(CLOCK_REALTIME, &ts_realtime_end);
        }

        int totalSetCount;
        MPI_Reduce((void *)&p->setCount,(void*)&totalSetCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        int totalGetCount;
        MPI_Reduce((void*)&p->getCount,(void*)&totalGetCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        sets[repeat] = totalSetCount;
        gets[repeat] = totalGetCount;


        if(procId == 0) {
            clock_gettime(CLOCK_REALTIME, &ts_realtime_end);
            time_data[repeat] = (double)(1e9*(ts_realtime_end.tv_sec-ts_realtime_start.tv_sec)+(ts_realtime_end.tv_nsec-ts_realtime_start.tv_nsec));
        }
    }

    if(procId == 0) {
        clock_gettime(CLOCK_REALTIME, &ts_totaltime_end);
        for(int repeat = 0; repeat < repeats; repeat++) {
            printf("%d,\t%d,\t%d,\t%d,\t%lf\n", repeat, results[repeat], sets[repeat], gets
                    [repeat], time_data[repeat]);
        }

        printf("Total time: %lf\n", (double)(1e9*(ts_totaltime_end.tv_sec-ts_totaltime_start.tv_sec)+(ts_totaltime_end.tv_nsec-ts_totaltime_start.tv_nsec)));
        //printPuzzle(p);
    }
    free(p->cell);
    free(p);
    MPI_Finalize();
    return 0;
}

