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
