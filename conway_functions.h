#define OFF '0'
#define ON '1'

// Count the number of live cells all around the cell given by
// the position (row, col).
int num_neighbours(char *b, int row, int col);

// initialize board reading from stdio
void initialize_board();

// free board memory
void free_board();

// print board
void print_board();

// to calculate the time
double rtclock();

// update the board according to the game of life rules
// serial implementation
void update_board_serial(char *b);

// allocs board2 and copies the value of board to board2
void initialize_board_2();

// copies the board2 to temp
void copy_board2_to_temp(); 

// return the number of different values between board and board2
int compare_serial_parallel();

// compare the serial and parallel results and time
void compare_serial(int n, double t_time_parallel);
