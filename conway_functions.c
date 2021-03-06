/*
 * MO644 Projeto Final 
 * 
 * Renan Gomes Pereira 103927
 * 
 * Conway's Game of Life
 * 
 * Functions called by the different game of life implementations
 */
 
#include "conway_functions.h"

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

void initialize_board(FILE *fp, int nr, int nc) {
    nrows = nr;
    ncols = nc;
    
    int i,j, id;
    
    board = (char *) malloc(nrows*ncols*sizeof(char));
    temp = (char *) malloc(nrows*ncols*sizeof(char));
    board_serial = NULL;
    
    for(i=0;i<nrows;i++) {
        for(j=0;j<ncols;j++) {
            id = i*ncols+j;
            
            fscanf(fp, "%c",&board[id]);
            temp[id] = board[id];
        }
    }
}

void free_board() {
    free(board);
    free(temp);
    
    if(board_serial != NULL) {
        free(board_serial);
    }
}

int num_neighbours(char *b, int row, int col) {
    int num_adj = 0;
    int i,j;
    
    for(i=row-1;i<=row+1;i++) {
        for(j=col-1;j<=col+1;j++) {
            // check boundaries and if the neighbour is alive
            if(i >= 0 && j>=0 && i < nrows && j < ncols && b[i*ncols+j] == ON)
                num_adj++;  
        }
    }
    
    // a cell is not a neighbour of itself 
    if(b[row*ncols+col] == ON)
        num_adj--; 
    
    return num_adj;
}

void update_board_serial(char *b, char *t) {
    int neighbours = 0;
    int id;
    
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            id = i*ncols+j;
            
            neighbours = num_neighbours(b, i, j);
            
            /* Dies by underpopulation. */
            if (neighbours < 2 && b[id] == ON) {
                t[id] = OFF; 
            } 
            /* Dies by overpopulation. */
            else if (neighbours > 3 && b[id] == ON) {
                t[id] = OFF; 
            }
            
            /* Become alive because of reproduction. */
            else if (neighbours == 3 && b[id] == OFF) {
                t[id] = ON;
            }
            
            /* Otherwise the cell lives with just the right company. */
            else {
                t[id] = b[id];
            }
        }
    }
}

void initialize_board_serial() {
    board_serial = (char *) malloc(nrows*ncols*sizeof(char));
    memcpy(&board_serial[0], &board[0], nrows*ncols*sizeof(char));
}

int compare_serial_parallel() {
    long diff=0;
    int i,j;
    
    for(i = 0 ; i < nrows; i++) 
        for(j = 0 ; j < ncols; j++)
            if(board[i*ncols+j] != board_serial[i*ncols+j])
                diff++;
    
    return diff;
}

void copy_board_serial_to_temp() {
    memcpy(&temp[0], &board_serial[0], nrows*ncols*sizeof(char));
}

void compare_serial(int n, double t_time_parallel) {
    double t_start, t_end, t_time_serial;
    
    copy_board_serial_to_temp();
        
    t_start = rtclock();
    
    int switch_boards = 0;
    
    for(int it = 0 ; it < n; it++) {
        if(!switch_boards) {
            update_board_serial(board_serial, temp);
            switch_boards = 1;
        }
        else {
            update_board_serial(temp, board_serial);
            switch_boards = 0;
        }
    }
    
    // copies the temp board back to the board
    if(n%2 != 0) {
        memcpy(&board_serial[0], &temp[0], nrows*ncols*sizeof(char));
    }
    
    t_end = rtclock();
    t_time_serial = t_end - t_start;
    
    printf("Time serial: %.3f seconds\n", t_time_serial);
    
    int diff = compare_serial_parallel();
    if(diff == 0) printf("Same result!\n");
    else printf("ERROR: Different result! Number of differences = %d\n", diff); 
    printf("Speedup: %.3f\n", t_time_serial/t_time_parallel); 
}
