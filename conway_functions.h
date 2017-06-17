/*
 * MO644 Projeto Final 
 * 
 * Renan Gomes Pereira 103927
 * Pedro Augusto Gebin de Carvalho 208618 
 * 
 * Conway's Game of Life
 * 
 * Game of life functions
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
 
// GLOBAL VARIABLES
char *board;
char *board_serial;
char *temp;
int nrows, ncols;

#define OFF '0'
#define ON '1'

// Runs the n iterations of the game of life
// Updates the board to the final state
// This functions is implemented in many different parallel approaches
void update_board(int n, int nt);

// Count the number of live cells all around the cell given by
// the position (row, col).
int num_neighbours(char *b, int row, int col);

// initialize board reading from stdio
void initialize_board(int nrows, int ncols);

// free board memory
void free_board();

// print board
void print_board();

// to calculate the time
double rtclock();

// update the board according to the game of life rules
// serial implementation
void update_board_serial(char *b);

// allocs board_serial and copies the value of board to board_serial
void initialize_board_serial();

// copies the board_serial to temp
void copy_board_serial_to_temp(); 

// return the number of different values between board and board_serial
int compare_serial_parallel();

// compare the serial and parallel results and time
void compare_serial(int n, double t_time_parallel);
