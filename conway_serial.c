/*
 * MO644 Projeto Final 
 * 
 * Renan Gomes Pereira 103927
 * 
 * Conway's Game of Life
 * 
 * Serial version
 */

#include "conway_functions.h"

void update_board(int n, int nt) {
    int switch_boards = 0;
    
    for(int it = 0 ; it < n; it++) {
        if(!switch_boards) {
            update_board_serial(board, temp);
            switch_boards = 1;
        }
        else {
            update_board_serial(temp, board);
            switch_boards = 0;
        }
    }
    
    // copies the temp board back to the board
    if(n%2 != 0) {
        memcpy(&board[0], &temp[0], nrows*ncols*sizeof(char));
    }
}
