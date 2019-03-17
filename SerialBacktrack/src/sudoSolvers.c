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

int btSolve(Puzzle * p)
{
    return(addGuess(p, 0, 0));
}

int addGuess(Puzzle * p, int x, int y)
{
    // If cell already has a value
    if(getCell(p, x, y) != 0) {
        int new_x, new_y;
        new_x = (x + 1) % p->degree;
        new_y = new_x == 0 ? (y + 1) : y;

        // If x and y are the last cell, we have a solution
        if(new_y == p->degree) return(verifyPuzzle(p));

        // Call addGuess. If it returns greater than 0, we have a solution
        // If it returns less than 0, this guess_val isn't valid
        return(addGuess(p, new_x, new_y) > 0); 
    }

    Cell guess_val = 1;
    do {
        // Try a number
        setCell(p, x, y, guess_val);

        // If valid, increment cell indexes and call addGuess
        if(verifyPuzzle(p) > 0) {
            int new_x, new_y;
            new_x = (x + 1) % p->degree;
            new_y = new_x == 0 ? (y + 1) : y;

            // If x and y are the last cell, we have a solution
            if(new_y == p->degree) return(1);

            // Call addGuess. If it returns greater than 0, we have a solution
            // If it returns less than 0, this guess_val isn't valid
            if(addGuess(p, new_x, new_y) > 0) 
                return(1);
        }
        guess_val++;
        if(guess_val > p->degree) { 
            setCell(p, x, y, 0);
            return(-1);
        }
    } while(1);
}

