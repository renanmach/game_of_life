/*
 * MO644 Projeto Final 
 * 
 * Renan Gomes Pereira 103927 
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
void initialize_board(FILE *fp, int nrows, int ncols);

// free board memory
void free_board();

// print board
void print_board();

// to calculate the time
double rtclock();

// return the number of different values between board and board_serial
int compare_serial_parallel();

// compare the serial and parallel results and time
void compare_serial(int n, double t_time_parallel);

// initialize board for serial comparison
void initialize_board_serial();

// updates the board with the serial approach
void update_board_serial(char *b, char *t);
