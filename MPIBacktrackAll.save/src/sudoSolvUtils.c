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


Puzzle * loadPuzzle(char *filename)
{
    int row, col;
    Cell v;
    int c;
    int degree;
    int count = 0;
    Puzzle *p;
    FILE *ifp;

    printf("Loading input file: %s\n", filename);
    ifp = fopen(filename,"r");

    if(ifp == NULL) {
        printf("ERROR: Unable to open file: %s\n", filename);
        return(NULL);
    }

    // First line of the puzzle file should be a single integer that is the puzzle degree
    if((c = fscanf(ifp, "%d\n", &degree)) != 1)
    {
        printf("Invalid puzzle file.\n");
        return(NULL);
    }
    if(degree < 2 || degree > 81) {
        printf("Invalid puzzle degree of %d. The degree must be between 2 and 81.\n", degree);
        return(NULL);
    }

    p = (Puzzle *)malloc(sizeof(Puzzle));
    p->degree = degree;
    p->setCount = count;
    p->getCount = count;
    p->solCnt = 0;
    p->zeroLevel = degree*degree;

    p->fixed_cells = (Cell *)malloc(sizeof(Cell)*degree*degree);
    Cell def_val = 0;
    for(unsigned int i = 0; i < degree*degree; i++) {
        p->fixed_cells[i] = def_val;
    }

    while(ifp) {
        if((c = fscanf(ifp, "%d,%d,%c\n", &row, &col, &v)) != 3) {
            break;
        }
        else {
            if(row > degree || col > degree) {
                printf("Bad cell coordinates: (%d, %d)\nExitting.\n", row, col);
                return(NULL);
            }
            v = v - '0';
            p->fixed_cells[row*degree+col] = v;
        }
    }
    fclose(ifp);

    p->dead_end = (SolNode *)malloc(sizeof(SolNode));
    p->dead_end->row = -1;
    p->dead_end->col = -1;
    p->dead_end->next_cells = NULL;

    p->solution_head = NULL;
    return(p);
}

int printSolution(Puzzle *p, Cell * s)
{
    Cell cv;

    // Print Stats

    for(unsigned int row = 0; row < p->degree; row++) {
        for(unsigned int col = 0; col < p->degree; col++) {
            cv = s[row*p->degree+col];
            (cv == 0) ? printf("  -") : printf("%3d", (unsigned int)cv);
        }
        printf("\n");
    }
    printf("\n");
    return(0);
}
int printPuzzle(Puzzle *p)
{
    Cell cv;

    // Print Stats
    for(unsigned int i = 0; i < p->degree; i++) printf("###"); printf("\n");
    printf("Degree = %d\tzeroDepth = %d\tsetCount = %d\tgetCount = %d\n", 
            p->degree,p->zeroLevel,p->setCount, p->getCount);

    for(unsigned int i = 0; i < p->degree; i++) printf("###");
    printf("\n");

    printSolution(p,p->fixed_cells);

    for(unsigned int i = 0; i < p->degree; i++) printf("###");
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

int checkPuzzleCol(int degree, Cell * s, int col)
{
    for(unsigned int row1 = 0; row1 < degree-1; row1++) {
        for(unsigned int row2 = row1+1; row2 < degree; row2++) {
            if(s[row1*degree+col] == s[row2*degree+col] && s[row1*degree+col] != 0) {
                return(-1);
            }
        }
    }
    return(1);
}

int checkPuzzleRow(int degree, Cell * s, int row)
{
    for(unsigned int col1 = 0; col1 < degree-1; col1++) {
        for(unsigned int col2 = col1+1; col2 < degree; col2++) {
            if(s[row*degree+col1] == s[row*degree+col2] && s[row*degree+col1] != 0) {
                return(-1);
            }
        }
    }
    return(1);
}

int checkPuzzleBlock(int degree, Cell * s, int k, int l) 
{
    int deg_sqrt = (int)sqrt(degree);
    for(unsigned int c1 = 0; c1 < degree-1; c1++) {
        unsigned int col1 = k*deg_sqrt + (c1 % deg_sqrt);
        unsigned int row1 = l*deg_sqrt + (c1 / deg_sqrt);
        for(unsigned int c2 = c1+1; c2 < degree; c2++) {
            unsigned int col2 = k*deg_sqrt + (c2 % deg_sqrt);
            unsigned int row2 = l*deg_sqrt + (c2 / deg_sqrt);
            if(s[row1*degree + col1] == s[row2*degree + col2] && s[row1*degree + col1] != 0) {
                return (-1);
            }
        }
    }
    return(1);
}


int verifySolution(int degree, Cell * s)
{
    // Check rows
    // For all rows
    // For all columns 0 to PDEGREE-1
    // For all columns  
    for(unsigned int row = 0; row < degree; row++) {
        if(checkPuzzleRow(degree, s, row) < 0) return(-1);
    }

    // Check columns
    // For all columns
    // For all rows 0 to degree-1
    // For all rows  
    for(unsigned int col = 0; col < degree; col++) {
        if(checkPuzzleCol(degree, s, col) < 0) return(-1);
    }

    // Check subblocks
    // Need to optimize this.
    // Loop for each subblock - there are PDEGREE subblocks
    int deg_sqrt = sqrt(degree);
    for(unsigned int k = 0; k < deg_sqrt; k++) {
        for(unsigned int l = 0; l < deg_sqrt; l++) {
            if(checkPuzzleBlock(degree, s, k, l) < 0) return(-1);
        }
    }
    return(1);
}

SolNode * createNode(Puzzle *p, int pos)
{
    SolNode * new_node;
    new_node = (SolNode *)malloc(sizeof(SolNode));
    new_node->row = pos / p->degree;
    new_node->col = pos - (new_node->row)*(p->degree);
    new_node->next_cells = (SolNode **)calloc((1 + p->degree), sizeof(SolNode*));
    return(new_node);
}

// If ex is set this is an exclusive node and if not zero, all other values are dead-ends
SolNode * addNodes(Puzzle *p, Cell * s, SolNode *node, int pos)
{
    unsigned int row = pos / p->degree;
    unsigned int col = pos - row*p->degree;

    // Return NULL if at the bottom
    if(row == p->degree) return(NULL);
    Cell val = s[pos];

    // Solution is empty
    if(node == NULL && pos != 0) return(NULL); 
    else if(node == NULL && pos == 0) {
        if(pos != 0) return(NULL);
        p->solution_head = createNode(p, pos);
        p->zeroLevel = p->degree*p->degree;
        node = p->solution_head;
        if(p->fixed_cells[pos] != 0) {
            for(unsigned int i = 0; i < p->degree+1; i++)
                if(i != val) node->next_cells[i] = p->dead_end;
        }
        else p->zeroLevel = 0;
    }

    
    // Invalid solution branch
    if(node->next_cells[val] == p->dead_end)
        return(p->dead_end);
    // Add node
    else if(node->next_cells[val] == NULL) {
        node->next_cells[val] = createNode(p, pos+1);
        // If this is an exclusive node, block all other values.
        if(p->fixed_cells[pos] != 0) {
            for(unsigned int i = 0; i < p->degree+1; i++)
                if(i != val) node->next_cells[i] = p->dead_end;
        }
        else if (p->zeroLevel > pos) p->zeroLevel = pos;
    }

    return(addNodes(p, s, node->next_cells[val], ++pos));  
}

int loadStartSolution(Puzzle * p, Cell * s)
{
    if(addNodes(p, s, p->solution_head, 0) == NULL)
        return(1);
    else
        return(-1);
}

int loadSolution(Puzzle * p, Cell * s)
{
    if(addNodes(p, s, p->solution_head, 0) == NULL)
        return(1);
    else
        return(-1);
}

void cleanUpTree(Puzzle *p, SolNode * sn)
{
    if(sn == NULL || sn == p->dead_end) return;
    for(unsigned int i = 0; i < p->degree+1; i++)
    {
        cleanUpTree(p, sn->next_cells[i]);
    }
    free(sn->next_cells);
    free(sn);
    return;
}


