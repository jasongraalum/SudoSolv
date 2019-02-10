// Copyright (c) 2019 Jason Graalum,
//
// CS 531 Performance Analysis
// Portland State University
// Winter 2019
//
// Jason Graalum
// Feb 2, 2019
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int solve(Puzzle * p)
{
    for(int x = 0; x < PDEGREE; x++) {
        for(int y = 0; y < PDEGREE; y ++) {
            if(getCell(p, x, y) == 0) {
                
            
}

int addGuess(Puzzle * p, int x, int y, Cell v)
{
    int deg_sqrt = (int)sqrt(PDEGREE);
    if(getCell(p, x, y) != 0) {
        x = (x + 1) % deq_sqrt;
        y = (x == 0) ? y++ : y;
        addGuess(p, x, y, 1);
    }
    else {
        

}

