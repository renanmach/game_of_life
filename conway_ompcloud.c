/*
 * MO644 Projeto Final 
 * 
 * Renan Gomes Pereira 103927
 * Pedro Augusto Gebin de Carvalho 208618 
 * 
 * Conway's Game of Life
 * 
 * OMPCloud verion
 */
 
#include "conway_functions.h"
#include <omp.h>

#define CLOUD 0

// GLOBAL VARIABLES
extern char *board;
extern char *temp;
extern int nrows, ncols;


void update_board(int n, int nt) {
    printf("Running OMPCloud!\n");
    
    #pragma omp target device(CLOUD) map(tofrom: board[:nrows*ncols],temp[:nrows*ncols])
    {
    
        for(int it = 0 ; it < n; it++) {
            
            #pragma omp parallel for
            for (int i = 0; i < nrows; i++) {
                for (int j = 0; j < ncols; j++) {
                    int id  = i*ncols + j;
                    int neighbours = num_neighbours(board, i, j);
                    
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
                    else {
                        temp[id] = board[id];
                    }
                }
            }
        }
    }

    if(n%2 != 0) {
        // copies the temp board back to the board
        memcpy(&board[0], &temp[0], nrows*ncols*sizeof(char));
    }
}
