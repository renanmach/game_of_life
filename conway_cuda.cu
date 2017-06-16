/*
 * MO644 Projeto Final 
 * 
 * Renan Gomes Pereira 103927
 * Pedro Augusto Gebin de Carvalho 208618 
 * 
 * Conway's Game of Life
 * 
 * CUDA version
 */

extern "C" {
    #include "conway_functions.h"
}
#include <stdio.h>
#include <string.h>

// TODO TESTAR VARIOS TILE WIDTH PARA O RELATORIO
#define TILE_WIDTH 16

extern char *board;
extern char *temp;
extern int nrows, ncols;

// cuda variables
char *d_board;
char *d_temp;
int board_size;

// update the board according to the game of life rules
void update_board();

// allocates and initialize cuda board and variables
void initialize_cuda_board();

int main(void) {
    double t_start, t_end;
    
    int n; // number of iteractions
    int nt; // number of threads (not used in cuda version)
    
    // read input
    scanf("%d %d", &n, &nt);
    scanf("%d %d",&nrows, &ncols);
    
    initialize_board();

    #ifdef COMPARE_SERIAL
        int n2 = n;
        initialize_board_2();
    #endif
    
    // run n iterations
    t_start = rtclock();
    initialize_cuda_board();
    while(n--) update_board();
    // copies the result back to the host
    cudaMemcpy(board, d_board, board_size, cudaMemcpyDeviceToHost);
    cudaFree(d_board);
    cudaFree(d_temp);
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

    free_board();
   
    return 0;
}

__device__ int num_neighbours_cuda(char * board, int row, int col, int nrows, int ncols) {
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

__global__ void update_board_cuda(char *board, char *temp, int nrows, int ncols) {
    int row = threadIdx.y + blockIdx.y * blockDim.y;
	int col = threadIdx.x + blockIdx.x * blockDim.x;
	int id = col + row*ncols;
    int neighbours;
    
    if (row < nrows && col < ncols) {
        neighbours = num_neighbours_cuda(board, row, col, nrows, ncols);
            
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

void initialize_cuda_board() {
    board_size = sizeof(char)*nrows*ncols;
    
    cudaMalloc((void **) &d_board, board_size);
    cudaMalloc((void **) &d_temp, board_size);
    
    cudaMemcpy(d_board, board, board_size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_temp, temp, board_size, cudaMemcpyHostToDevice);
}

void update_board() {
    dim3 dimGrid(ceil(ncols/(float)TILE_WIDTH), ceil(nrows/(float) TILE_WIDTH));
	dim3 dimBlock(TILE_WIDTH, TILE_WIDTH);
    
    update_board_cuda<<<dimGrid,dimBlock>>>(d_board, d_temp, ncols, nrows);
    // TODO TESTAR SEM
    cudaThreadSynchronize();
}
