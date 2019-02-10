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
    char *inFile;

    if(argc > 1)
    {
        inFile = (char*)malloc(sizeof(char)*(strlen(argv[1])+1));
        strncpy(inFile,argv[1],strlen(argv[1])+1);
    }
    else
    {
        inFile = (char*)malloc(sizeof(char)*(strlen(defFile)+1));
        strncpy(inFile,defFile,strlen(defFile)+1);
    }

    printf("Input file: %s\n", inFile);

    printf("Loading puzzle\n");
    p = loadPuzzle(inFile);
    if (p == NULL) {
        printf("Error loading puzzle. Exitting.\n");
    }

    printf("Printing start puzzle\n");
    printPuzzle(p);

    if(treeSolve(p)) {
        printf("The puzzle is solved\n");
    }
    else {
        printf("Unable to solve the puzzle.\n");
    }

    printPuzzle(p);

    if(verifyPuzzle(p) > 0) 
        printf("Puzzle solution is valid.\n");
    else
        printf("Puzzle solution is invalid.\n");

	return 0;
}

