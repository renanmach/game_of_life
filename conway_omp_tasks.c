/*
 * MO644 Projeto Final 
 * 
 * Renan Gomes Pereira 103927
 * 
 * Conway's Game of Life
 * 
 * OMP Tasks version
 */
 
#include "conway_functions.h"
#include <omp.h>

// GLOBAL VARIABLES
extern char *board;
extern char *temp;
extern int nrows, ncols;

void run_iteration(char *board, char *temp) {
    // create parallel region
    #pragma omp parallel
    {   
        // a single thread runs this loop
        #pragma omp single
        for (int i = 0; i < nrows; i++) {
            // create a task
            #pragma omp task
            {
                for (int j = 0; j < ncols; j++) {
                    int neighbours = num_neighbours(board, i, j);
                    int id = i*ncols + j;
                    
                    /* Dies by underpopulation. */
                    if (neighbours < 2 && board[id] == ON) {
                        temp[id] = OFF; 
                    } 
                    /* Dies by overpopulation. */
                    else if (neighbours > 3 && board[id] == ON) {
                        temp[id] = OFF; 
                    }
                    
                    /* Become alive because of reproduction. */
                    else if (neighbours == 3 && board[id] == OFF) {
                        temp[id] = ON;
                    }
                    
                    /* Otherwise the cell lives with just the right company. */
                }
            }
        }
    }
    
    // copies the temp board back to the board
   memcpy(&board[0], &temp[0], nrows*ncols*sizeof(char));
}

void update_board(int n, int nt) {
    printf("Running OMP tasks!\n");
    
    int switch_boards = 0;
    
    omp_set_num_threads(nt);
    
    for(int it = 0 ; it < n; it++) {
        if(!switch_boards) {
            run_iteration(board, temp);
            switch_boards = 1;
        }
        
        else {
            run_iteration(temp, board);
            switch_boards = 0;
        }
    }
    
    if(n%2 != 0) {
        // copies the temp board back to the board
        memcpy(&board[0], &temp[0], nrows*ncols*sizeof(char));
    }
}
