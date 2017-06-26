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
#define NINPUT_FILES 3

char input_files[NINPUT_FILES][100] = {"input/input01.dat","input/input02.dat","input/input03.dat"};

int main(void) {
    double t_start, t_end, t_time;
    int n; // number of iteractions
    int nt; // number of threads
    int nrows,ncols,i;
    FILE *fp;
    
    for(i = 0 ; i < NINPUT_FILES; i++) {
        fp = fopen(input_files[i], "r");
        printf("\n");
        printf("%s -------- ------\n", input_files[i]);
        printf("\n");
        
        // read input
        fscanf(fp, "%d %d", &n, &nt);
        fscanf(fp, "%d %d",&nrows, &ncols);
        
        initialize_board(fp, nrows,ncols);
        
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
        
        printf("Time: %.3f seconds\n", t_time);
        
        // Compares the board with the serial version
        // Prints the speedup
        #ifdef COMPARE_SERIAL
        compare_serial(n, t_time);
        #endif
        
        free_board();
        fclose(fp);
    }
   
    return 0;
}
