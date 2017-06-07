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
 
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

// GLOBAL VARIABLES
char **board;
char **temp;
int nrows, ncols;

#define OFF 0
#define ON 1

// Count the number of live cells all around the cell given by
// the position (row, col).
int num_neighbours(int row, int col);

// initialize board reading from stdio
void initialize_board();

// free board memory
void free_board();

// print board
void print_board();

// to calculate the time
double rtclock();

// update the board according to the game of life rules
void update_board();

int main(void) {
    double t_start, t_end;
    
    int n; // number of iteractions
    int nt; // number of threads (not used in serial version)
    
    // read input
    scanf("%d %d", &n, &nt);
    scanf("%d %d",&nrows, &ncols);
    
    omp_set_num_threads(nt);
    
    initialize_board();
    
    t_start = rtclock();
    while(n--) update_board();
    t_end = rtclock();
    
    double t_time = t_end - t_start;
   
    print_board();
    printf("Time: %f seconds\n", t_time);
    
    free_board();
   
    return 0;
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

void update_board() {
    int neighbours = 0;
    
    #pragma omp parallel for private(neighbours)
    for (int y = 0; y < nrows; y++) {
        for (int x = 0; x < ncols; x++) {
            neighbours = num_neighbours(x, y);
            
            /* Dies by underpopulation. */
            if (neighbours < 2 && board[x][y] == ON) {
                temp[x][y] = OFF; 
            } 
            /* Dies by overpopulation. */
            else if (neighbours > 3 && board[x][y] == ON) {
                temp[x][y] = OFF; 
            }
            
            /* Become alive because of reproduction. */
            else if (neighbours == 3 && board[x][y] == OFF) {
                temp[x][y] = ON;
            }
            
            /* Otherwise the cell lives with just the right company. */
        }
    }
    
    // copies the temp board back to the board
    for (int y = 0; y < nrows; y++) {
        for (int x = 0; x < ncols; x++) {
            board[x][y] = temp[x][y];
        }
    }
}

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
