// Copyright (c) 2019 Jason Graalum, //
// CS 531 Performance Analysis
// Portland State University
// Winter 2019
//
// Jason Graalum
// Feb 2, 2019
//

#include "sudoSolvers.h"
#include "sudoSolvUtils.h"

int btAllSolver(Puzzle * p, int numProcs)
{
    int degree = p->degree;
    int solCount = 0;
    int busyProcs = 0;
    int noMore = 0;

    trial_t * nextTry = NULL;
    solEntry_t ** procCurrentTry = (solEntry_t **)calloc(numProcs, sizeof(solEntry_t *));

    procFreeQ_head_t procFreeQ_head;
    TAILQ_INIT(&procFreeQ_head);

    solQ_head_t solQ_head;
    TAILQ_INIT(&solQ_head);

    // Initial free process queue
    for(int i = 1; i < numProcs; i++) {
        struct procEntry * newProc = malloc(sizeof(struct procEntry));
        newProc->procId = i;
        newProc->numProcs = numProcs;
        TAILQ_INSERT_TAIL(&procFreeQ_head, newProc, entries); 
    }

    if(TAILQ_EMPTY(&procFreeQ_head)) {
        printf("Process pool is empty. Exitting.\n");
        return(-1);
    }

    printPuzzle(p);
    if(verifySolution(degree, p->fixed_cells) <0) return(-1);

    // Load starting puzzle into the tree.
    loadStartSolution(p, p->fixed_cells);

    // Check that the original puzzle has at lest one solution.
    Cell * solution = (Cell *)malloc(sizeof(Cell)*degree*degree);
    memcpy(solution, p->fixed_cells, sizeof(Cell)*degree*degree);
    int result = btSolve(degree, solution);
    free(solution);

    // Build n possible solution options(based on number of processes)
    // by finding the first n NULL pointers
    procEntry_t * nextProc;
    
    do {
        // Add next possible solution to the solution queue
        if(noMore == 0) {
            // Start with fixed cells
            Cell * new_solution = (Cell *)malloc(sizeof(Cell)*degree*degree);
            memcpy(new_solution, p->fixed_cells, sizeof(Cell)*degree*degree);

            // Get next possible solution
            nextTry = getNextSolution(p, p->solution_head, new_solution, 0);

            // If a valid solution(not a dead_end), insert into solution queue
            if(nextTry->sn != NULL && nextTry->sn != p->dead_end)
            {
                solEntry_t * newSol = malloc(sizeof(struct solEntry));
                newSol->trial = nextTry;
                solCount++;
                TAILQ_INSERT_TAIL(&solQ_head, newSol, entries);
            }
        }

        // While there are open processors and solutions on the queue
        // Send them out.
        while(!TAILQ_EMPTY(&procFreeQ_head) && !TAILQ_EMPTY(&solQ_head))  {
            // Get the next available process
            nextProc = TAILQ_FIRST(&procFreeQ_head);
            TAILQ_REMOVE(&procFreeQ_head, nextProc, entries);

            // Get the next trial solution
            solEntry_t * newSol = TAILQ_FIRST(&solQ_head);
            TAILQ_REMOVE(&solQ_head, newSol, entries);

            // Send the degree of the puzzle
            MPI_Send(&degree, 
                    1, 
                    MPI_INT, 
                    nextProc->procId, 
                    0, 
                    MPI_COMM_WORLD);

            //Save off trial details in case it is a failure.
            procCurrentTry[nextProc->procId] = newSol;
            busyProcs++;

            // Send solution
            MPI_Send(newSol->trial->solution, 
                    degree, 
                    mpiCell, 
                    nextProc->procId, 
                    0, 
                    MPI_COMM_WORLD);
        }

        // If any processes are running, wait for the solution
        if(busyProcs > 0) {

            // Wait for a recv from any the then requeue that proc
            // status var is needed to get the sending procId
            //int result = btSolve(degree, nextTry->solution);
            MPI_Status status;
            Cell * sol = (Cell *)malloc(sizeof(Cell)*(degree*degree+1));

            MPI_Recv(sol, degree*degree+1, mpiCell, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

            int sendingProc = status.MPI_SOURCE;

            if(sol[0] > 0) {
                p->solCnt++;
                printf("Received good solution:%d\n",p->solCnt);
                printSolution(p, sol+1);
                loadSolution(p, sol+1);
            }
            else
            {
                printf("Received bad solution\n");
                int index = procCurrentTry[sendingProc]->trial->index;
                procCurrentTry[sendingProc]->trial->sn->next_cells[index] = p->dead_end;
            }
            free(procCurrentTry[sendingProc]->trial->solution);
            free(procCurrentTry[sendingProc]->trial);
            free(procCurrentTry[sendingProc]); 
            procCurrentTry[sendingProc] = NULL;

            nextProc->procId = status.MPI_SOURCE;
            TAILQ_INSERT_TAIL(&procFreeQ_head, nextProc, entries);
            busyProcs--;
        }
        if(nextTry->sn == NULL) noMore=1;

    } while(nextTry->sn != NULL || busyProcs > 0); 

    printf("Solution trial count = %d\n", solCount);
    int termSignal = -1;
    MPI_Send((void*)&termSignal, 
            1, 
            MPI_INT, 
            nextProc->procId, 
            0, 
            MPI_COMM_WORLD);

    return(0);
}
trial_t * getNextSolution(Puzzle *p, SolNode * sn, Cell * s, int pos)
{

    int degree = p->degree;
    trial_t * nextTry;
    // Error condition
    if(sn == NULL) exit(-1);//(NULL);

    // Position has a fixed cell value, move down to next position
    if(s[pos] != 0)
    {
        nextTry = getNextSolution(p, sn->next_cells[s[pos]], s, pos+1);
        if(nextTry->sn == NULL) {
            //printf("No solution to puzzle.\n");
            sn->next_cells[s[pos]] = p->dead_end;
        }
        return(nextTry);
    }

    // Level has open values
    //printf("pos = %d : ",pos);
    if(sn->next_cells[0] != p->dead_end)
    {

        // Get first open index
        for(int i = 1; i < degree+1; i++) {
            if(sn->next_cells[i] != p->dead_end)
            {
                if(sn->next_cells[i] == NULL) {
                    //printf("(S)pos = %d : index = %d\n", pos, i);
                    nextTry = (trial_t *)malloc(sizeof(trial_t));
                    s[pos] = i;
                    nextTry->pos = pos;
                    nextTry->index = i;
                    nextTry->sn = sn;
                    nextTry->solution = s;
                    //printf("s[0] = %d\n", s[0]);
                    return(nextTry);
                }
                else {
                    s[pos] = i;
                    //printf("(C)pos = %d : index = %d\n", pos, i);
                    trial_t * nextTry = getNextSolution(p, sn->next_cells[s[pos]], s, pos+1);
                    if(nextTry->sn == NULL) sn->next_cells[s[pos]] = p->dead_end;
                    else return(nextTry);
                }
            }
        }
        // No open index - set this level to dead_end
        sn->next_cells[0] = p->dead_end;
        nextTry = (trial_t *)malloc(sizeof(trial_t));
        nextTry->pos = pos;
        nextTry->index = 0;
        nextTry->sn = NULL;
        nextTry->solution = NULL;
        return(nextTry);
    }
                    nextTry = (trial_t *)malloc(sizeof(trial_t));
    nextTry->pos = pos;
    nextTry->index = 0;
    nextTry->sn = NULL; //p->dead_end;
    nextTry->solution = NULL;
    return(nextTry);

}
/*
int buildNextSolution(Puzzle *p, SolNode * sn, Cell * s, int pos, procFreeQ_head_t * qhead)
{
    if(sn == NULL || sn == p->dead_end || pos == p->degree*p->degree) return(-1);
    // If 0th element is dead_end this node is filled out.
    // Starting with the first cell value, look further for a open node
    if(s[pos] != 0) {
        if(pos == p->degree*p->degree) return(-1);
        return(buildNextSolution(p, sn->next_cells[s[pos]], s, pos+1, qhead));
    }
    else if(sn->next_cells[0] == p->dead_end)
    {
        for(unsigned int i = 1; i < p->degree+1; i++) {
            if(sn->next_cells[i] != p->dead_end) { 
                s[pos] = i;
                // Only return 1 if it was able to "try" a new solution
                int nextTry = buildNextSolution(p, sn->next_cells[i], s, pos+1, qhead);
                if(nextTry == 1) return(1);
            }
        }
        sn->next_cells[0] = p->dead_end;
        // There are no open nodes
        return(-1);
    } else {
        SolNode * zeroNode = sn->next_cells[0];
        SolNode * newNode;
        // 0th element is NOT dead_end, which means this node is open    
        // Find the first NULL in the next_cells
        struct procEntry * nextProc;
        for(unsigned int i = 1; i < p->degree+1; i++) {
            if(sn->next_cells[i] == NULL) {
                // Set the solution to i
                s[pos] = i;
                nextProc = TAILQ_FIRST(qhead);
                if(nextProc)  printf("procId = %d\n", nextProc->procId);
                TAILQ_REMOVE(qhead,nextProc,entries);

                if(btSolve(p->degree, s)> 0) {
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
                s[pos] = 0;
                if(nextProc)
                    TAILQ_INSERT_TAIL(qhead,nextProc,entries);
            }
        }
        sn->next_cells[0] = p->dead_end;
        return(1);
    }
}
*/

/*
void *solveDispatch() {

    solEntry_t * nextTryn;
    procEntry_t * nextTryn;
    int wait_time = 1000;
    while(1) {
        if(!TAILQ_EMPTY(&solQ_head))
        {
            while(TAILQ_EMPTY(&procFreeQ_head)) {
                wait(wait_time);
                wait_time +=100;
            }
            nextProc = TAILQ_FIRST(&procFreeQ_head);
            nextTryn = TAILQ_FIRST(&solQ_head);

            if(nextTryn->degree == -1) {
                //wait for all MPI procs to finish
                // Use the procQ to decided when they have all returned 
                break;
            }

            MPI_Send(&(nextTryn->degree), 1, MPI_INT, 0, MPI_COMM_WORLD);

            TAILQ_REMOVE(&solQ_head, nextTryn, entries);
            TAILQ_REMOVE(&procFreeQ_head, nextProc, entries);

    }

}
*/


