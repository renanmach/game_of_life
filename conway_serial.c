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

#include "conway_functions.h"

void update_board(int n, int nt) {
    for(int it = 0; it < n; it++)
        update_board_serial(board);
}
