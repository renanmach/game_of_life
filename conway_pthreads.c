/*
 * MO644 Projeto Final 
 * 
 * Renan Gomes Pereira 103927
 * Pedro Augusto Gebin de Carvalho 208618 
 * 
 * Conway's Game of Life
 * 
 * Pthreads version
 */

#include "conway_functions.h"
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// GLOBAL VARIABLES
extern char *board;
extern char *temp;
extern char *board2; // for serial comparison
extern int nrows, ncols;
int NUM_THREADS;
pthread_t *thread_handles;

// update the board according to the game of life rules
void update_board();

int main(void) {
    double t_start, t_end;
    
    int n; // number of iteractions
    int nt; // number of threads
    
    // read input
    scanf("%d %d", &n, &nt);
    scanf("%d %d",&nrows, &ncols);
    
    // for global use by pthreads
    NUM_THREADS = nt;
    thread_handles = (pthread_t *) malloc(sizeof(pthread_t) * NUM_THREADS);
    
    initialize_board();
    
    #ifdef COMPARE_SERIAL
    int n2 = n;
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
    
    //Pthreads
    free(thread_handles);
    free_board();
   
    return 0;
}

// function called for each thread
void *update_board_parallel(void *arguments) {
    long rank = (long) arguments;
    
    int start = rank*nrows/NUM_THREADS;
    int end = rank == (NUM_THREADS-1) ? nrows : (rank+1)*nrows/NUM_THREADS;
    
    int neighbours, id;
    
    for (int i = start; i < end; i++) {
        for (int j = 0; j < ncols; j++) {
            id = i*ncols + j;
            neighbours = num_neighbours(i, j);
            
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
    
    return NULL;
}

void update_board() {
    long thread;
	
	for(thread = 0; thread < NUM_THREADS; thread++) {
		pthread_create(&thread_handles[thread], NULL, update_board_parallel, (void *) thread);
	}
	
	for(thread = 0 ; thread < NUM_THREADS; thread++) {
		pthread_join(thread_handles[thread], NULL);
	}
    
    // copies the temp board back to the board
    memcpy(&board[0], &temp[0], nrows*ncols*sizeof(char)); 
}
