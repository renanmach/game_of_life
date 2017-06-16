/*
 * MO644 Projeto Final 
 * 
 * Renan Gomes Pereira 103927
 * Pedro Augusto Gebin de Carvalho 208618 
 * 
 * Conway's Game of Life
 * 
 * Serial version
 */

#include "conway_functions.h"
#include <stdio.h>
#include <string.h>

extern char *board;
extern char *temp;
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
    update_board_serial();
}
