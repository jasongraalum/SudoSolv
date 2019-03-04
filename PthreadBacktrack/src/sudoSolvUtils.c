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

size_t xlatRowCol(int row, int col, int degree)
{
    if(row >= 0 &&
            row < degree &&
            col >= 0 &&
            col < degree)
    {
        return(row*degree + col);
        //return(col/sizeof(Cell) + row*degree/sizeof(Cell));
    }
    return(-1);
}


int setCell(int row, int col, int v)
{
    size_t pos = xlatRowCol(row, col, p->degree);
    if(pos >= 0)
    {
        Cell val = 0 | (1 << v);
        //printf("Setting (%d, %d)%d to %lx\n", row, col, pos, val);
        p->cell[pos] = val;
        //p->cell[pos] = (Cell)(0x00000000 | (1 << v));
        p->setCount++;
        return(1);
    }
    else
        return(-1);
}

int getCell(int row, int col)
{
    if(p == NULL) return(-1);
    size_t pos = xlatRowCol(row, col, p->degree);
    int result = -1;
    if(pos >= 0)
    {
       p->getCount++;
       result = log2((double)p->cell[pos]);
    }
    return(result);
}

int loadPuzzle(char *filename)
{
    int row, col;
    int v;
    int c;
    int degree;
    int count = 0;
    FILE *ifp;

    printf("Loading input file: %s\n", filename);
    ifp = fopen(filename,"r");

    if(ifp == NULL) {
        printf("ERROR: Unable to open file: %s\n", filename);
        return(-1);
    }

    // First line of the puzzle file should be a single integer that is the puzzle degree
    if((c = fscanf(ifp, "%d\n", &degree)) != 1)
    {
        printf("Invalid puzzle file.\n");
        return(-1);
    }
    
    if(degree < 2 || degree > 81) {
        printf("Invalid puzzle degree of %d. The degree must be between 2 and 81.\n", degree);
        return(-1);
    }

    p = (Puzzle *)malloc(sizeof(Puzzle));
    p->cell = malloc(sizeof(Cell)*degree*degree);
    p->degree = degree;
    p->setCount = count;
    p->getCount = count;

    int def_v = 0;
    for(int row = 0; row < degree; row++)
        for(int col = 0; col < degree; col++)
        {
            setCell(row, col, def_v);
        }

    
    while(ifp)
    {
        if((c = fscanf(ifp, "%d,%d,%d\n", &row, &col, &v)) != 3)
        {
            break;
        }
        else 
        {
            if(row > degree || col > degree) {
                printf("Bad cell coordinates: (%d, %d)\nExitting.\n", row, col);
                return(-1);
            }
            setCell(row, col, v);
        }
    }
    fclose(ifp);
    return(1);
}

int printPuzzle()
{
    Cell cv;

    for(int i = 0; i < p->degree; i++) printf("###");
    printf("\n");
    printf("Degree = %d\tsetCount = %d\tgetCount = %d\n", p->degree,p->setCount, p->getCount);
    for(int i = 0; i < p->degree; i++) printf("###");
    printf("\n");
    for(int row = 0; row < p->degree; row++) {
        for(int col = 0; col < p->degree; col++) {
            cv = getCell(row, col);
            ((long int)cv == 0) ? printf("  -") : printf("%3d", (int)cv);
        }
        printf("\n");
    }
    for(int i = 0; i < p->degree; i++) printf("###");
    printf("\n");
    return(0);
}

int getBlockRow(int row, int degree)
{
    return(row % (int)sqrt(degree));
}

int getBlockCol(int col, int degree)
{
    return(col % (int)sqrt(degree));
}

int checkPuzzleCol(int col)
{
    for(int row1 = 0; row1 < p->degree-1; row1++) {
        for(int row2 = row1+1; row2 < p->degree; row2++) {
            if(getCell(row1, col) == getCell(row2, col) && getCell(row1, col) != 0) {
                //printf("Column Error at (%d, %d) and (%d, %d)\n", row1, col, row2, col);
                return(-1);
            }
        }
    }
    return(1);
}

void * checkPuzzleCols()
{
    int * retval = (int *)malloc(sizeof(int));;
    *retval = -1;
    for(int col = 0; col < p->degree; col++) {
        if(checkPuzzleCol(col) < 0) return((void *)retval);
    }
    *retval = 1;
    pthread_exit((void *)retval);
}

int checkPuzzleRow(int row)
{
    for(int col1 = 0; col1 < p->degree-1; col1++) {
        for(int col2 = col1+1; col2 < p->degree; col2++) {
            if(getCell(row, col1) == getCell(row, col2) && getCell(row, col1) != 0) {
                //printf("Row Error at (%d, %d) and (%d, %d)\n", row, col1, row, col2);
                return(-1);
            }
        }
    }
    return(1);
}

void * checkPuzzleRows()
{
    int * retval = (int *)malloc(sizeof(int));;
    *retval = -2;
    for(int row = 0; row < p->degree; row++) {
        if(checkPuzzleRow(row) < 0) pthread_exit((void *)retval);
    }
    *retval = 2;
    pthread_exit((void *)retval);
}


int checkPuzzleBlock(int k, int l) 
{
    int deg_sqrt = (int)sqrt(p->degree);
    for(int c1 = 0; c1 < p->degree-1; c1++) {
        int col1 = k*deg_sqrt + (c1 % deg_sqrt);
        int row1 = l*deg_sqrt + (c1 / deg_sqrt);
        for(int c2 = c1+1; c2 < p->degree; c2++) {
            int col2 = k*deg_sqrt + (c2 % deg_sqrt);
            int row2 = l*deg_sqrt + (c2 / deg_sqrt);
            if(getCell(row1, col1) == getCell(row2, col2) && getCell(row1, col1) != 0) {
                //printf("Subblock Error at (%d, %d) and (%d, %d)\n", row1, col1, row2, col2);
                return (-1);
            }
        }
    }
    return(1);
}

void * checkPuzzleBlocks()
{
    int * retval = (int *)malloc(sizeof(int));;
    *retval = -4;
    int deg_sqrt = sqrt(p->degree);
    for(int k = 0; k < deg_sqrt; k++) {
        for(int l = 0; l < deg_sqrt; l++) {
            if(checkPuzzleBlock(k, l) < 0) pthread_exit((void *) retval);
        }
    }
    *retval = 4;
    pthread_exit((void *) retval);

}


int verifyPuzzle()
{
    void *row_result, *col_result, *block_result;

    pthread_t *tid_row, *tid_col, *tid_block;
    tid_row = (pthread_t *)malloc(sizeof(pthread_t));
    tid_col = (pthread_t *)malloc(sizeof(pthread_t));
    tid_block = (pthread_t *)malloc(sizeof(pthread_t));

    // Check rows
    // For all rows
    // For all columns 0 to PDEGREE-1
    // For all columns  
    pthread_create(tid_row, NULL, checkPuzzleRows, NULL);

    // Check columns
    // For all columns
    // For all rows 0 to degree-1
    // For all rows  
    pthread_create(tid_col, NULL, checkPuzzleCols, NULL);

    // Check subblocks
    // Need to optimize this.
    // Loop for each subblock - there are PDEGREE subblocks
    pthread_create(tid_block, NULL, checkPuzzleBlocks, NULL);

    pthread_join(*tid_row, &row_result);
    pthread_join(*tid_col, &col_result);
    pthread_join(*tid_block, &block_result);

    int result = ((*(int *)row_result) + (*(int*)col_result) + (*(int *)block_result));

    return(result-6);
}
