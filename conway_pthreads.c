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
#include <pthread.h>

extern char *board;
extern char *temp;
extern int nrows, ncols;

// Pthreads variables
int NUM_THREADS;

// function called for each thread
void *run_iteration(void *arguments) {
    long rank = (long) arguments;
    
    int start = rank*nrows/NUM_THREADS;
    int end = rank == (NUM_THREADS-1) ? nrows : (rank+1)*nrows/NUM_THREADS;
    
    int neighbours, id;
    
    for (int i = start; i < end; i++) {
        for (int j = 0; j < ncols; j++) {
            id = i*ncols + j;
            neighbours = num_neighbours(board, i, j);
            
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

void update_board(int n, int nt) {
    printf("Running Pthreads!\n");
    
    long thread;
    pthread_t *thread_handles;
    thread_handles = (pthread_t *) malloc(sizeof(pthread_t) * nt);
    
    // for global use by pthreads
    NUM_THREADS = nt;
    
    // runs n iterations
    while(n--) {
        for(thread = 0; thread < nt; thread++) {
            pthread_create(&thread_handles[thread], NULL, run_iteration, (void *) thread);
        }
        
        for(thread = 0 ; thread < nt; thread++) {
            pthread_join(thread_handles[thread], NULL);
        }
        
        // copies the temp board back to the board
        memcpy(&board[0], &temp[0], nrows*ncols*sizeof(char)); 
    }
    
    free(thread_handles);
}
