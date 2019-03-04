# SUDOSolv

<p>
Course Project<br>
CS531 Winter 2019 <br>
Portland State University<br>
</p>

Jason Graalum
February 2, 2019

## Overview
This is a simple, contrived testcase to predict, measure and analyse the performance of a set of C program which solve the same problem - namely finding the solution Sudoku puzzles of varying diffculty. The goals of the program are to develop three programs which generate all solutions to any given Sudoku puzzle:

1. Single threaded naive solution
1. Multi threaded, single processor solution
1. Multi threaded, multi-processor solution

For each solution, I will analyze the algorithm and program in order to predict the performance. When I will write the program and will measure and analyze the actual performance. Finally, I will compare the predicted performace to the actual performance with the goal of identifying the causes of any differences seen.

## Sudoku Rules
Sudoku is [a logic-based, combinatorial number placement puzzle.](https://en.wikipedia.org/wiki/Sudoku) The puzzle is made up of a 9x9 grid of cells. When solved, each cell contains a single digit between 1 and 9 inclusive. The numbers in each row of 9 and in each column of 9 must be unique. Also, when the 9x9 grid is segmented into a 3x3 grid with each cell containing a 3x3 grid of cells, the number in each 3x3 "sub-cell" must also be unique.

![Example of a Sudoku puzzle](https://upload.wikimedia.org/wikipedia/commons/thumb/e/e0/Sudoku_Puzzle_by_L2G-20050714_standardized_layout.svg/240px-Sudoku_Puzzle_by_L2G-20050714_standardized_layout.svg.png)


When starting a puzzle a selection of the cells are pre-filled with digits. This provides a starting point for the solution. Ideally, there is a unique solution given the starting point. However, this can not always be guaranteed. (A stretch goal of this project is to quickly enumerate the number of solutions to a given puzzle.) 

## Puzzle Constraints and Complexity
To solve a puzzle, there are two methods I will explore. The first is a very naive trial-and-error process. Given the 81 cells, the puzzle can be represented as a tree graph with constraints between cells in common rows, columns and sub-blocks.   

As a toy example, consider a reduce puzzle with a 4x4 grid and digits from 1 to 4. Then, we have the following 
constraints:

Given a cell (x,y), where x is the row number(1 -> 4)k and y is the column number(1 -> 4) and v(x,y) is the digita value in that cell, then the following must hold:
1. v(x,y) != v(i,y) | i = 0 -> 3 ^ i != x
1. v(x,y) != v(x,j) | j = 0 -> 3 ^ j != y
1. v(x,y) != v(i,j) | i = x * mod 2 * (0 -> 1) ^
j = y * mod 2 * (0 -> 1) ^
(i != x) \\/ (j != y)

To generalize this, we can set the size of the puzzle to N which is the row and column size. (In the above case, N = 2). The generalized constraints are:

1. v(x,y) != v(i,y) | i = 1 -> N ^ i != x
1. v(x,y) != v(x,j) | j = 1 -> N ^ j != y
1. v(x,y) != v(i,j) | i = (x-1) * mod sqrt(N) * (1 -> sqrt(N)) ^
j = (y-1) * mod sqrt(N) * (1 -> sqrt(N)) ^
(i != x) \\/ (j != y)

As initial estimate of the complexity may be found by enumerating the comparisons needed. Looking at the three constraints per cell:
1. N
2. N
3. sqrt(N) * sqrt(N) + 2

Per cell complexity is O(N + N + N) = O(N). Given N^2 cells the total complexity is O(N^3)


[1]: <https://en.wikipedia.org/wiki/Sudoku>

