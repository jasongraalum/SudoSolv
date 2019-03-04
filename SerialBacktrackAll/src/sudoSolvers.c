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

int btAllSolve(Puzzle * p, int numProcs)
{
    printPuzzle(p);
    if(verifySolution(p, p->fixed_cells) <0) return(-1);

    // Load starting puzzle into the tree.
    loadStartSolution(p, p->fixed_cells);

    // Check that the original puzzle has at lest one solution.
    Cell * solution = (Cell *)malloc(sizeof(Cell)*p->degree*p->degree);
    memcpy(solution, p->fixed_cells, sizeof(Cell)*p->degree*p->degree);
    int result = btSolve(p, solution);
    printSolution(p, solution);

    if(result) loadSolution(p, solution);
    else return(-1);

    // Build n possible solution options(based on number of processes)
    // by finding the first n NULL pointers
    int pos = 0;
    Cell * newsol = (Cell *)malloc(sizeof(Cell)*p->degree*p->degree);
    memcpy(newsol, p->fixed_cells, sizeof(Cell)*p->degree*p->degree);

    buildNextSolution(p, p->solution_head, newsol, 0); 
    printSolution(p, newsol);
    free(newsol);
    free(solution);
    
    return(0);
}
int buildNextSolution(Puzzle *p, SolNode * sn, Cell * s, int pos)
{
    if(sn == NULL || sn == p->dead_end) return(-1);
    // If 0th element is dead_end this node is filled out.
    // Starting with the first cell value, look further for a open node
    if(sn->next_cells[0] == p->dead_end)
    {
        for(unsigned int i = 1; i < p->degree+1; i++) {
            if(sn->next_cells[i] != p->dead_end) { 
                s[pos] = i;
                return(buildNextSolution(p, sn->next_cells[i], s, pos+1));
            }
        }
        // There are no open nodes
        return(-1);
    } else {
        SolNode * zeroNode = sn->next_cells[0];
        SolNode * newNode;
        // 0th element is NOT dead_end, which means this node is open    
        // Find the first NULL in the next_cells
        for(unsigned int i = 1; i < p->degree+1; i++) {
            if(sn->next_cells[i] == NULL) {
                // Set the solution to i
                s[pos] = i;
                return(1);
            }
        }
        return(-1);

    }
    return(-1);
}

int btSolve(Puzzle * p, Cell * s)
{
    return(addGuess(p, s, 0, 0));
}

int addGuess(Puzzle * p, Cell * s, int row, int col)
{
    int pos = row*p->degree + col;
    // If cell already has a value
    if(s[pos] != 0) {
        int new_row, new_col;
        new_row = (row + 1) % p->degree;
        new_col = new_row == 0 ? (col + 1) : col;

        // If row and col are the last cell, we have a solution
        if(new_col == p->degree) return(verifySolution(p,s));

        // Call addGuess. If it returns greater than 0, we have a solution
        // If it returns less than 0, this guess_val isn't valid
        return(addGuess(p, s, new_row, new_col) > 0); 
    }

    Cell guess_val = 1;
    do {
        // Try a number
        int pos = row*p->degree + col;
        s[pos] = guess_val;

        // If valid, increment cell indexes and call addGuess
        if(verifySolution(p,s) > 0) {
            int new_row, new_col;
            new_row = (row + 1) % p->degree;
            new_col = new_row == 0 ? (col + 1) : col;

            // If row and col are the last cell, we have a solution
            if(new_col == p->degree) return(1);

            // Call addGuess. If it returns greater than 0, we have a solution
            // If it returns less than 0, this guess_val isn't valid
            if(addGuess(p, s, new_row, new_col) > 0) 
                return(1);
        }
        guess_val++;
        if(guess_val > p->degree) { 
            pos = row*p->degree + col;
            s[pos] = 0;
            return(-1);
        }
    } while(1);
}




