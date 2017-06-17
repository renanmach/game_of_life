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

// TODO TESTAR VARIOS TILE WIDTH PARA O RELATORIO ***********************************
#define TILE_WIDTH 16

extern char *board;
extern char *temp;
extern int nrows, ncols;

// cuda variables
char *d_board;
char *d_temp;
int board_size;

// allocates and initialize cuda board and variables
void initialize_cuda_board() {
    board_size = sizeof(char)*nrows*ncols;
    
    cudaMalloc((void **) &d_board, board_size);
    cudaMalloc((void **) &d_temp, board_size);
    
    cudaMemcpy(d_board, board, board_size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_temp, temp, board_size, cudaMemcpyHostToDevice);
}

__device__ int num_neighbours_cuda(char *board, int row, int col, int nrows, int ncols) {
    int num_adj = 0;
    int i,j;
    
    for(i=row-1;i<=row+1;i++) {
        for(j=col-1;j<=col+1;j++) {
            // check boundaries and if the neighbour is alive
            if(i >= 0 && j>=0 && i < nrows && j < ncols && board[i*ncols+j] == ON)
                num_adj++;  
        }
    }
    
    // a cell is not a neighbour of itself 
    if(board[row*ncols+col] == ON)
        num_adj--; 
    
    return num_adj;
}

__global__ void copy_temp_to_board(char *board, char *temp, int nrows, int ncols) {
    int row = threadIdx.y + blockIdx.y * blockDim.y;
	int col = threadIdx.x + blockIdx.x * blockDim.x;
	int id = col + row*ncols;
    
    if (row < nrows && col < ncols) {
        board[id] = temp[id];
    }
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
        else {
            temp[id] = board[id];
        }
	}
}

void update_board(int n, int nt) {
    printf("Running CUDA!\n");
    
    // switch boards so we dont have to copy temp to board every time
    int switch_boards = 0;
    
    initialize_cuda_board();
    
    dim3 dimGrid(ceil(ncols/(float)TILE_WIDTH), ceil(nrows/(float) TILE_WIDTH));
	dim3 dimBlock(TILE_WIDTH, TILE_WIDTH);
    
    for(int it = 0; it < n; it++) {
        if(switch_boards) {
            update_board_cuda<<<dimGrid,dimBlock>>>(d_board, d_temp, nrows, ncols);
            switch_boards = 0;
        }
        
        else {
            update_board_cuda<<<dimGrid,dimBlock>>>(d_temp, d_board, nrows, ncols);
            switch_boards = 1;
        }
        
        cudaThreadSynchronize();
        
        //copy_temp_to_board<<<dimGrid,dimBlock>>>(d_board, d_temp, nrows, ncols);
    }
    
    
    
    // copies the result back to the host
    if(n%2 != 0) {
        cudaMemcpy(board, d_board, board_size, cudaMemcpyDeviceToHost);
    }
    
    else {
        cudaMemcpy(board, d_temp, board_size, cudaMemcpyDeviceToHost);
        copy_temp_to_board<<<dimGrid,dimBlock>>>(d_board, d_temp, nrows, ncols);
    }
    
    cudaFree(d_board);
    cudaFree(d_temp);
}
