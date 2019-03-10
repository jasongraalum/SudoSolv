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

TAILQ_HEAD(snTailhead, snEntry) snHead;
struct snEntry {
    int pos;
    int degree;
    Cell * solution;
    TAILQ_ENTRY(snEntry) snEntries;
};

TAILQ_HEAD(procTailhead, procEntry) procHead;
struct procEntry {
    int procId;
    int numProcs;
    TAILQ_ENTRY(procEntry) procEntries;
};

int btAllSolve(Puzzle * p, int numProcs)
{
    printPuzzle(p);
    if(verifySolution(p->degree, p->fixed_cells) <0) return(-1);

    // Load starting puzzle into the tree.
    loadStartSolution(p, p->fixed_cells);

    // Check that the original puzzle has at lest one solution.
    Cell * solution = (Cell *)malloc(sizeof(Cell)*p->degree*p->degree);
    memcpy(solution, p->fixed_cells, sizeof(Cell)*p->degree*p->degree);
    int result = btSolve(p->degree, solution);
    //printSolution(p, solution);

    //if(result) loadSolution(p, solution);
    //else return(-1);

    // Build n possible solution options(based on number of processes)
    // by finding the first n NULL pointers
    int pos = 0;
    int retval;
    Cell * newsol = (Cell *)malloc(sizeof(Cell)*p->degree*p->degree);
    do {
        memcpy(newsol, p->fixed_cells, sizeof(Cell)*p->degree*p->degree);
        retval = buildNextSolution(p, p->solution_head, newsol, 0);
    } while(retval != -1); 

    free(newsol);
    free(solution);
    
    return(0);
}
int buildNextSolution(Puzzle *p, SolNode * sn, Cell * s, int pos)
{
    if(sn == NULL || sn == p->dead_end || pos == p->degree*p->degree) return(-1);
    // If 0th element is dead_end this node is filled out.
    // Starting with the first cell value, look further for a open node
    if(s[pos] != 0) {
        if(pos == p->degree*p->degree) return(-1);
        return(buildNextSolution(p, sn->next_cells[s[pos]], s, pos+1));
    }
    else if(sn->next_cells[0] == p->dead_end)
    {
        for(unsigned int i = 1; i < p->degree+1; i++) {
            if(sn->next_cells[i] != p->dead_end) { 
                s[pos] = i;
                // Only return 1 if it was able to "try" a new solution
                int retval = buildNextSolution(p, sn->next_cells[i], s, pos+1);
                if(retval != -1) return(retval);
            }
        }
        sn->next_cells[0] = p->dead_end;

        // There are no open nodes
        return(-1);
    } else {

        return(pos);
        // 0th element is NOT dead_end, which means this node is open    
        // Find the first NULL in the next_cells
        for(unsigned int i = 1; i < p->degree+1; i++) {
            if(sn->next_cells[i] == NULL) {
                // Set the solution to i
                s[pos] = i;

                struct snEntry *newSnEntry;
                newSnEntry = malloc(sizeof(struct snEntry));
                newSnEntry->solution = malloc(sizeof(Cell)*p->degree*p->degree));
                newSnEntry->pos = pos;
                newSnEntry->solution = sn;
                TAILQ_INSERT_HEAD(&snHead, newSnEntry, snEntries);

                /*

                if(btSolve(p, s) > 0) {
                    p->solCnt++;
                    printf("Found new solution:%d\n",p->solCnt);
                    printSolution(p, s);
                    loadSolution(p, s);
                }
                else
                {
                    //printf("Found bad solution\n");
                    //printSolution(p, s);
                    sn->next_cells[i] = p->dead_end;
                    if(i == p->degree) 
                        sn->next_cells[0] = p->dead_end;
                }
                }
                */
                s[pos] = 0;
            }
        }
        sn->next_cells[0] = p->dead_end;
        return(1);
    }
}

int solverDispatch(int numProcs)
{

    /*
    // Initialize queue with available MPI processes
    struct procEntry *newProc;
    for(int i = 0; i < numProcs; i++) {
        newProc = malloc(sizeof(struct snEntry));
        newProc->procId = i;
        newProc->numProcs = numProcs;
        TAILQ_INSERT_HEAD(&procHead, newProc, procEntries);
    }

    struct procEntry * nextProc;
    struct snEntry * entry malloc(sizeof(struct snEntry));
    int waitVal = 0;
    // Infinite loop - will extra when a specific value is enqueued
    while(1) {
        // If status is > 0, there is a free processor. If not, wait for a while and try again
        if((nextProc = TAILQ_FIRST(&procHead)) != NULL) {
            waitVal = 0;
            while((snEntry = TAILQ_FIRST(&snHead)) == NULL) {
                waitVal += 100;
                wait(waitVal);
            }
            waitVal = 0;
            // Look for sentinel value
            if(snEntry->pos == -1) return(0);
            MPI_Send(&(snEntry->degree), 1, MPI_INT, nextProc->procId, MPI_COMM_WORLD); 
            MPI_Send(&(snEntry->degree), 1, MPI_INT, nextProc->procId, MPI_COMM_WORLD); 
        }
        else {
            void * retval;
            MPI_Recv(&retval,1, MPI_INT, 
            waitVal += 100;
            wait(waitVal);
        }

    }
    */
    return(0);
}
