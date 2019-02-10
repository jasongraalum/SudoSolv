// Copyright (c) 2019 Jason Graalum,
//
// CS 531 Performance Analysis // Portland State University
// Winter 2019
//
// Jason Graalum
// Feb 2, 2019
//
// sudoSolv Utilities 
//

#include <sudoSolvUtils.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


size_t xlatRowCol(int row, int col)
{
    if(row >= 0 &&
            row < PDEGREE &&
            col >= 0 &&
            col < PDEGREE)
    {
        return(col*sizeof(Cell)*PDEGREE + row*sizeof(Cell));
    }
    return(-1);
}


int setCell(Puzzle *p, int row, int col, Cell v)
{
    size_t pos = xlatRowCol(row, col);
    if(pos >= 0)
        p->cell[pos] = v;
    else
        return(-1);
    return(1);
}

Cell getCell(Puzzle *p, int row, int col)
{
    size_t pos = xlatRowCol(row, col);
    if(pos >= 0)
        return(p->cell[xlatRowCol(row,col)]);
    else
        return(pos);
}


int loadPuzzle(char *filename, Puzzle * p)
{
    printf("Loading input file: %s\n", filename);
    FILE *ifp;
    ifp = fopen(filename,"r");


    if(ifp == NULL) {
        printf("ERROR: Unable to open file: %s\n", filename);
        return(-1);
    }

    int row, col, v;
    int c;
    int count = 0;
    while(ifp)
    {
        if((c = fscanf(ifp, "%d,%d,%d\n", &row, &col, &v)) != 3)
        {
            break;
        }
        else 
        {
            count += c/3;
            setCell(p, row, col, v);
        }
    }
    return(count);

}

int printPuzzle(Puzzle *p)
{
    Cell cv;
    for(int row = 0; row < PDEGREE; row++) {
        for(int col = 0; col < PDEGREE; col++) {
            cv = getCell(p, row, col);
            ((int)cv == 0) ? printf(" - ") : printf(" %d ", (int)cv);
        }
        printf("\n");
    }
    return(0);
}

int initPuzzle(Puzzle *p)
{
    for(int row = 0; row < PDEGREE; row++) {
        for(int col = 0; col < PDEGREE; col++) {
            setCell(p, row, col, (Cell)0);
        }
    }
    return(0);
}

int getBlockRow(int x)
{
    return(x % (int)sqrt(PDEGREE));
}

int getBlockCol(int y)
{
    return(y % (int)sqrt(PDEGREE));
}

int checkPuzzleRow(Puzzle * p, int x)
{
    for(int y = 0; y < PDEGREE-1; y++) {
        for(int j = y+1; j < PDEGREE; j++) {
            if(getCell(p, x, y) == getCell(p, x, j) && getCell(p, x, y) != 0) {
                printf("Row Error at (%d, %d) and (%d, %d)\n", x, y, x, j);
                return(-1);
            }
        }
    }
    return(1);
}

int checkPuzzleCol(Puzzle * p, int y)
{
    for(int x = 0; x < PDEGREE-1; x++) {
        for(int i = x+1; i < PDEGREE; i++) {
            if(getCell(p, x, y) == getCell(p, i, y) && getCell(p, x, y) != 0) {
                printf("Column Error at (%d, %d) and (%d, %d)\n", x, y, i, y);
                return(-1);
            }
        }
    }
    return(1);
}

int checkPuzzleBlock(Puzzle *p, int k, int l) 
{
    int deg_sqrt = sqrt(PDEGREE);
    for(int i = k*deg_sqrt; i < (k+1)*deg_sqrt; i++) {
        for(int j = l*deg_sqrt; j < (l+1)*deg_sqrt; j++) {

            for(int x = k*deg_sqrt; x < (k+1)*deg_sqrt; x++) {
                for(int y = l*deg_sqrt; y < (l+1)*deg_sqrt; y++) {
                    if(((i != x) || (j != y)) && getCell(p, x, y) == getCell(p, i, j) && getCell(p, x, y != 0)) {
                        printf("Subblock Error at (%d, %d) and (%d, %d)\n", i, j, x, y);
                        return (-1);
                    }
                }
            }
        }
    }
    return(1);
}


int verifyPuzzle(Puzzle * p)
{
    // Check rows
    // For all rows
    // For all columns 0 to PDEGREE-1
    // For all columns  
    for(int x = 0; x < PDEGREE; x++) {
        if(checkPuzzleRow(p, x) < 0) return(-1);
    }

    // Check columns
    // For all columns
    // For all rows 0 to PDEGREE-1
    // For all rows  
    for(int y = 0; y < PDEGREE; y++) {
        if(checkPuzzleCol(p, y) < 0) return(-1);
    }

    // Check subblocks
    // Need to optimize this.
    // Loop for each subblock - there are PDEGREE subblocks
    int deg_sqrt = sqrt(PDEGREE);
    for(int k = 0; k < deg_sqrt; k++) {
        for(int l = 0; l < deg_sqrt; l++) {
            if(checkPuzzleBlock(p, k, l) < 0) return(-1);
        }
    }
    return(1);
}

