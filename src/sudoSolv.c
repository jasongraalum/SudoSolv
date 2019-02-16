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

#include <sudoSolvUtils.h>
#include <sudoSolvers.h>

int main(int argc, char **argv)
{
    Puzzle *p;

    const char *defFile = "test/test.pz";
    char *inFilename;

    if(argc > 1)
    {
        inFilename = (char*)malloc(sizeof(char)*(strlen(argv[1])+1));
        strncpy(inFilename,argv[1],strlen(argv[1])+1);
    }
    else
    {
        inFilename = (char*)malloc(sizeof(char)*(strlen(defFile)+1));
        strncpy(inFilename,defFile,strlen(defFile)+1);
    }

    printf("Input file: %s\n", inFilename);

    printf("Loading puzzle\n");
    p = loadPuzzle(inFilename);
    if (p == NULL) {
        printf("Error loading puzzle. Exitting.\n");
    }

    printf("Printing start puzzle\n");
    printPuzzle(p);

    if(treeSolve(p)) {
        printf("The puzzle was solved in %d sets and %d gets.\n", p->setCount, p->getCount);
    }
    else {
        printf("Unable to solve the puzzle after %d sets and %d gets.\n", p->setCount, p->getCount);
    }

    printPuzzle(p);

    if(verifyPuzzle(p) > 0) 
        printf("Puzzle solution is valid.\n");
    else
        printf("Puzzle solution is invalid.\n");

    free(p);
	return 0;
    
}

