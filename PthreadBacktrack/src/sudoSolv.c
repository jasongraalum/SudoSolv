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

#include <sudoSolvUtils.h>
#include <sudoSolvers.h>

int main(int argc, char **argv)
{

    char *inFilename;
    char *solver;
    int repeats;

    struct timespec ts_totaltime_start;
    struct timespec ts_totaltime_end;

    struct timespec ts_realtime_start;
    struct timespec ts_realtime_end;

    clock_gettime(CLOCK_REALTIME, &ts_totaltime_start);
    double *time_data;
    int *results;
    int *sets;
    int *gets;

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
    results = (int *)malloc(sizeof(int)*repeats);
    sets = (int *)malloc(sizeof(int)*repeats); 
    gets = (int *)malloc(sizeof(int)*repeats);

    for(int repeat = 0; repeat < repeats; repeat++) {

        // Setup and print starting puzzle
        if(loadPuzzle(inFilename)< 0) {
            printf("Error loading puzzle. Exitting.\n");
        }

        clock_gettime(CLOCK_REALTIME, &ts_realtime_start);
        results[repeat] = btSolve();
        sets[repeat] = p->setCount;
        gets[repeat] = p->getCount;
        clock_gettime(CLOCK_REALTIME, &ts_realtime_end);

        time_data[repeat] = (double)(1e9*(ts_realtime_end.tv_sec-ts_realtime_start.tv_sec)+(ts_realtime_end.tv_nsec-ts_realtime_start.tv_nsec));
    }
    clock_gettime(CLOCK_REALTIME, &ts_totaltime_end);

    for(int repeat = 0; repeat < repeats; repeat++) {
        printf("%d,\t%d,\t%d,\t%d,\t%lf\n", repeat, results[repeat], sets[repeat], gets[repeat], time_data[repeat]);
    }
    
    printf("Total time: %lf\n", (double)(1e9*(ts_totaltime_end.tv_sec-ts_totaltime_start.tv_sec)+(ts_totaltime_end.tv_nsec-ts_totaltime_start.tv_nsec)));

    printPuzzle(p);
    free(p->cell);
    free(p);
    free(time_data);
    free(results);
    free(sets);
    free(gets);
    return 0;

}

