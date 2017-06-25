/*
 * MO644 Projeto Final 
 * 
 * Renan Gomes Pereira 103927 
 * 
 * Conway's Game of Life
 * 
 * Main code
 */
 
#include "conway_functions.h"

int main(void) {
    double t_start, t_end, t_time;
    
    int n; // number of iteractions
    int nt; // number of threads
    int nrows,ncols;
    
    // read input
    scanf("%d %d", &n, &nt);
    scanf("%d %d",&nrows, &ncols);
    
    initialize_board(nrows,ncols);
    
    #ifdef COMPARE_SERIAL
    initialize_board_serial();
    #endif
    
    // run n iterations
    t_start = rtclock();
    update_board(n,nt);
    t_end = rtclock();
    
    t_time = t_end - t_start;
   
    #ifdef PRINT_BOARD
    print_board();
    #endif
    
    printf("Time: %f seconds\n", t_time);
    
    // Run serial version and compare with parallel results
    // Prints the speedup
    #ifdef COMPARE_SERIAL
    compare_serial(n, t_time);
    #endif
    
    free_board();
   
    return 0;
}
