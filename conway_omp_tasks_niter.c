/*
 * MO644 Projeto Final 
 * 
 * Renan Gomes Pereira 103927
 * Pedro Augusto Gebin de Carvalho 208618 
 * 
 * Conway's Game of Life
 * 
 * OMP Tasks version
 */
 
#include "conway_functions.h"
#include <stdio.h>
#include <omp.h>

// GLOBAL VARIABLES
extern char **board;
extern char **temp;
extern int nrows, ncols;

// update the board according to the game of life rules
void update_board();

int main(void) {
    double t_start, t_end;
    
    int n; // number of iteractions
    int nt; // number of threads (not used in serial version)
    
    // read input
    scanf("%d %d", &n, &nt);
    scanf("%d %d",&nrows, &ncols);
    
    omp_set_num_threads(nt);
    
    initialize_board();
    
    // run n iterations
    t_start = rtclock();
    while(n--) update_board();
    t_end = rtclock();
    
    double t_time = t_end - t_start;
   
    print_board();
    printf("Time: %f seconds\n", t_time);
    
    free_board();
   
    return 0;
}

void update_board() {
    // create parallel region
    #pragma omp parallel
    {   
        // a single thread runs this loop
        #pragma omp single
        for (int y = 0; y < nrows; y++) {
            // create a task
            #pragma omp task
            {
                for (int x = 0; x < ncols; x++) {
                    int neighbours = num_neighbours(x, y);
                    
                    /* Dies by underpopulation. */
                    if (neighbours < 2 && board[x][y] == ON) {
                        temp[x][y] = OFF; 
                    } 
                    /* Dies by overpopulation. */
                    else if (neighbours > 3 && board[x][y] == ON) {
                        temp[x][y] = OFF; 
                    }
                    
                    /* Become alive because of reproduction. */
                    else if (neighbours == 3 && board[x][y] == OFF) {
                        temp[x][y] = ON;
                    }
                    
                    /* Otherwise the cell lives with just the right company. */
                }
            }
        }
    }
    
    // copies the temp board back to the board
    for (int y = 0; y < nrows; y++) {
        for (int x = 0; x < ncols; x++) {
            board[x][y] = temp[x][y];
        }
    }
}
