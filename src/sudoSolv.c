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
#include <sudoSolvUtils.h>
#include <string.h>

int main(int argc, char **argv)
{
	Puzzle *p;
    p = (Puzzle *)malloc(sizeof(Puzzle));
    char defFile[] = "test/test.pz";
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

    initPuzzle(p);
    printPuzzle(p);

    printf("Loading puzzle\n");
    printf("Loaded %d values\n", loadPuzzle(inFile,p));

    printf("Printing puzzle\n");
    printPuzzle(p);

    if(verifyPuzzle(p) > 0) 
        printf("Puzzle solution is valid.\n");
    else
        printf("Puzzle solution is invalid.\n");

	return 0;
}

