/*
 * MO644 Projeto Final 
 * 
 * Renan Gomes Pereira 103927
 * Pedro Augusto Gebin de Carvalho 208618 
 * 
 * Conway's Game of Life
 * 
 * Create test case
 */
 
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define NROWS (int) 1000
#define NCOLS (int) 1000
#define FILE_NAME "test_files/input/1.txt"
#define N_THREADS (int) 1
#define N_ITER (int) 100

// Input format
// [number of iteractions] [number of threads]
// [nrows] [ncols]
// n lines with n integers 0 or 1 representing the initial grid

int main(void) {
    srand(time(NULL));
    
    int i, j;
    int c;
    
    FILE *fp;
    
    fp = fopen(FILE_NAME, "w");
    fprintf(fp,"%d %d\n", N_ITER, N_THREADS);
    fprintf(fp,"%d %d\n", NROWS, NCOLS);
    
    
    for(i = 0 ; i < NROWS ; i++) {
        for(j = 0 ; j < NCOLS ; j++) {
            c = (int)(rand() % 2);
            fprintf(fp,"%d",c);
        }
        fprintf(fp,"\n");
    }
    
    fclose(fp);
    
    return 0;
}
