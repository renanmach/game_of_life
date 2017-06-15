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
char **board;
char **board2;
char **temp;
int nrows, ncols;

void print_board() {
    int i,j;
    
    for(i=0;i<nrows;i++) {
        for(j=0;j<ncols;j++) {
            printf("%c",board[i][j]);
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
    int i,j;
    
    board = (char **) malloc(nrows*sizeof(char*));
    temp = (char **) malloc(nrows*sizeof(char*));
    board2 = NULL;
    
    for(i=0;i<nrows;i++) {
        board[i] = (char *) malloc(ncols*sizeof(char));
        temp[i] = (char *) malloc(ncols*sizeof(char));
        
        for(j=0;j<ncols;j++) {
            scanf("%c",&board[i][j]);
            temp[i][j] = board[i][j];
        }
    }
}

void free_board() {
    int i;
    
    for(i=0;i<nrows;i++) {
        free(board[i]);
        free(temp[i]);
    }
    
    free(board);
    free(temp);
    
    if(board2 != NULL) {
        for(i=0;i<nrows;i++) {
            free(board2[i]);
        } 
        free(board2);
    }
}

int num_neighbours(int row, int col) {
    int num_adj = 0;
    int i,j;
    
    for(i=row-1;i<=row+1;i++) {
        for(j=col-1;j<=col+1;j++) {
            if(i==j) continue;
            
            if(i >= 0 && j>=0 && i < nrows && j < ncols && board[i][j] == ON)
                num_adj++;  
        }
    }
    
    return num_adj;
}

void update_board_serial() {
    int neighbours = 0;
    
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            neighbours = num_neighbours(i, j);
            
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
    
    // copies the temp board back to the board
    int line_size = ncols*sizeof(char);
    for (int i = 0; i < nrows; i++) {
        memcpy((&board[i][0]),(&temp[i][0]),line_size);
    }
}

void initialize_board_2() {
    int i,j;
    
    board2 = (char **) malloc(nrows*sizeof(char*));
    
    for(i=0;i<nrows;i++) {
        board2[i] = (char *) malloc(ncols*sizeof(char));
        
        for(j=0;j<ncols;j++) {
            board2[i][j] = board[i][j];
            temp[i][j] = board[i][j];
        }
    }
}

int compare_serial_parallel() {
    long diff=0;
    int i,j;
    
    for(i = 0 ; i < nrows; i++) 
        for(j = 0 ; j < ncols; j++)
            if(board[i][j] != board2[i][j])
                diff++;
    
    return diff;
}

void copy_board2_to_temp() {
    for(int i = 0 ; i < nrows ; i++) {
        for(int j = 0; j < ncols; j++) {
            temp[i][j] = board2[i][j];
        }
    }
}
