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
#include <string.h>

// GLOBAL VARIABLES
extern char **board;
extern char **temp;
extern char **board2; // for serial comparison
extern int nrows, ncols;

// update the board according to the game of life rules
void update_board();

int main(void) {
    double t_start, t_end;
    
    int n, n2; // number of iteractions
    int nt; // number of threads
    
    // read input
    scanf("%d %d", &n, &nt);
    scanf("%d %d",&nrows, &ncols);
    
    // for serial comparison
    n2 = n;
    
    omp_set_num_threads(nt);
    
    initialize_board();
    
    #ifdef COMPARE_SERIAL
    initialize_board_2();
    #endif
    
    // run n iterations
    t_start = rtclock();
    while(n--) update_board();
    t_end = rtclock();
    
    double t_time = t_end - t_start;
   
    #ifdef PRINT_BOARD
    print_board();
    #endif
    
    printf("Time: %f seconds\n", t_time);
    
    // Run serial version and compare with parallel results
    // Prints the speedup
    #ifdef COMPARE_SERIAL
    copy_board2_to_temp();
    
    t_start = rtclock();
    while(n2--) update_board_serial();
    t_end = rtclock();
    double t_time_serial = t_end - t_start;
    printf("Time serial: %f seconds\n", t_time_serial);
    printf("Speedup: %f seconds\n", t_time_serial/t_time);
    
    int diff = compare_serial_parallel();
    if(diff == 0) printf("Same result!\n");
    else printf("ERROR: Different result! Number of differences = %d\n", diff);
    #endif
    
    free_board();
   
    return 0;
}

void update_board() {
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
                    int neighbours = num_neighbours(i, j);
                    
                    /* Dies by underpopulation. */
                    if (neighbours < 2 && board[i][j] == ON) {
                        temp[i][j] = OFF; 
                    } 
                    /* Dies by overpopulation. */
                    else if (neighbours > 3 && board[i][j] == ON) {
                        temp[i][j] = OFF; 
                    }
                    
                    /* Become alive because of reproduction. */
                    else if (neighbours == 3 && board[i][j] == OFF) {
                        temp[i][j] = ON;
                    }
                    
                    /* Otherwise the cell lives with just the right company. */
                }
            }
        }
    }
    
    // copies the temp board back to the board
    int line_size = ncols*sizeof(char);
    for (int i = 0; i < nrows; i++) {
        memcpy((&board[i][0]),(&temp[i][0]),line_size);
    }
}
