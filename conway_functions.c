#include "conway_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// GLOBAL VARIABLES
char **board;
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
