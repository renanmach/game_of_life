/*
 * MO644 Projeto Final 
 * 
 * Renan Gomes Pereira 103927
 * Pedro Augusto Gebin de Carvalho 208618 
 * 
 * Conway's Game of Life
 * 
 * Functions called by the different game of life implementations
 */
#include "conway_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

// GLOBAL VARIABLES
char *board;
char *board2;
char *temp;
int nrows, ncols;

void print_board() {
    int i,j;
    
    for(i=0;i<nrows;i++) {
        for(j=0;j<ncols;j++) {
            printf("%c",board[i*ncols + j]);
        }
        printf("\n");
    }
}

double rtclock() {
  struct timezone Tzp;
  struct timeval Tp;
  int stat;
  stat = gettimeofday (&Tp, &Tzp);
  if (stat != 0) printf("Error return from gettimeofday: %d",stat);
  return(Tp.tv_sec + Tp.tv_usec*1.0e-6);
}

void initialize_board() {
    int i,j, id;
    
    board = (char *) malloc(nrows*ncols*sizeof(char*));
    temp = (char *) malloc(nrows*ncols*sizeof(char*));
    board2 = NULL;
    
    for(i=0;i<nrows;i++) {
        for(j=0;j<ncols;j++) {
            id = i*ncols+j;
            
            scanf("%c",&board[id]);
            temp[id] = board[id];
        }
    }
}

void free_board() {
    free(board);
    free(temp);
    
    if(board2 != NULL) {
        free(board2);
    }
}

int num_neighbours(int row, int col) {
    int num_adj = 0;
    int i,j;
    
    for(i=row-1;i<=row+1;i++) {
        for(j=col-1;j<=col+1;j++) {
            if(i==row && j == col) continue;
            
            if(i >= 0 && j>=0 && i < nrows && j < ncols && board[i*ncols+j] == ON)
                num_adj++;  
        }
    }
    
    return num_adj;
}

void update_board_serial() {
    int neighbours = 0;
    int id;
    
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            id = i*ncols+j;
            
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
    
    // copies the temp board back to the board
    memcpy(&board[0], &temp[0], nrows*ncols*sizeof(char)); 
}

void initialize_board_2() {
    board2 = (char *) malloc(nrows*ncols*sizeof(char*));
    memcpy(&board2[0], &board[0], nrows*ncols*sizeof(char));
}

int compare_serial_parallel() {
    long diff=0;
    int i,j;
    
    for(i = 0 ; i < nrows; i++) 
        for(j = 0 ; j < ncols; j++)
            if(board[i*ncols+j] != board2[i*ncols+j])
                diff++;
    
    return diff;
}

void copy_board2_to_temp() {
    memcpy(&temp[0], &board2[0], nrows*ncols*sizeof(char));
}
