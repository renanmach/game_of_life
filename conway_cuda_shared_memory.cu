/*
 * MO644 Projeto Final 
 * 
 * Renan Gomes Pereira 103927
 * 
 * Conway's Game of Life
 * 
 * CUDA shared memory version
 */

extern "C" {
    #include "conway_functions.h"
}

#define TILE_WIDTH 16

// for shared memory
#define MASK_WIDTH 3
#define RADIUS (MASK_WIDTH-1)/2
#define SM_LINE_SIZE (TILE_WIDTH+MASK_WIDTH-1)
#define SM_SIZE SM_LINE_SIZE*SM_LINE_SIZE

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

__device__ inline int num_neighbours_cuda(char *board) {
    int num_adj = 0;
    int i,j;
    
    for(i = threadIdx.y; i < MASK_WIDTH+threadIdx.y; i++) {
        for(j = threadIdx.x; j < MASK_WIDTH+threadIdx.x; j++) {
            if(board[i*SM_LINE_SIZE + j] == ON)
                num_adj++; 
        }
    }
        
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
    __shared__ char shared_board_part[SM_SIZE];
    
    int row = threadIdx.y + blockIdx.y * blockDim.y;
	int col = threadIdx.x + blockIdx.x * blockDim.x;
	int id = col + row*ncols;
    int neighbours, curr_r, curr_c, mapID;
    
    // fills the shared memory array
	for(curr_r=row-RADIUS; curr_r-row+RADIUS+threadIdx.y < MASK_WIDTH-1+TILE_WIDTH; curr_r += TILE_WIDTH) {	
		for(curr_c=col-RADIUS; curr_c-col+RADIUS+threadIdx.x < MASK_WIDTH-1+TILE_WIDTH; curr_c += TILE_WIDTH) {
			mapID = SM_LINE_SIZE*(curr_r-row+RADIUS + threadIdx.y) + curr_c-col+RADIUS + threadIdx.x;
			
			if(curr_c >= 0 && curr_c < ncols && curr_r >= 0 && curr_r < nrows) {
                shared_board_part[mapID] = board[curr_r*ncols + curr_c];
			}
			
			// borders
			else {
				shared_board_part[mapID] = 0;
			}
		}
	}

	__syncthreads();
    
    if (row < nrows && col < ncols) {
        neighbours = num_neighbours_cuda(shared_board_part);
        
        // a cell is not a neighbour of itself 
        if(board[id] == ON) neighbours--;
        
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
    printf("Running CUDA shared!\n");
    
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
