/*
 * MO644 Projeto Final 
 * 
 * Renan Gomes Pereira 103927
 * Pedro Augusto Gebin de Carvalho 208618 
 * 
 * Conway's Game of Life
 * 
 * 
 * Modification of
 * https://github.com/zsck/C.-Game-of-Life
 * by ryuurei
 */
 
#define DELAY 0
#define ROWS 60
#define COLS 60
#define scr_width 600
#define scr_height 600

#define OFF 0
#define ON 1
#define BLACK 8, 0, 0, 0, 0
#define cell_width (scr_width / ROWS)
#define cell_height (scr_height / COLS)

// Clear the board then randomly set ~1/5 of them to ON.
void randomize_board(void);

// Position horizontal and veritcal lines to form a grid to
// make it easier to count cell spaces when trying to draw things.
void initialize_grid(SDL_Surface* screen);

// Blit all the live cells onto the board.
void blit_board(SDL_Surface* bcell, SDL_Surface* screen);

// Count the number of live cells all around the cell given by
// the position (x, y). Of course, this includes all diagonal, vertical,
// and horizontal living cells. The program works using a toroidal array,
// thus we must check to see if it is necessary to wrap to the other side
// of the array in order to find cells that must be taken into account.
int num_neighbours(int x, int y);

// Determine which cells live and which die. Operate on temp so that
// each change does not affect following changes, then copy temp into
// the main board to be displayed.
void update_board(void);

// Simply set both the main and temporary boards to completely off.
int clear_board(SDL_Surface* screen, Uint32 color);

/* Clear a cell by coloring it white and setting the board pos. to off. */
int clear_cell(SDL_Surface* screen, int x, int y, Uint32 color);

// Initialize the array of SDL_Rect structs that store the cell
// coordinates by iteratively calculating their successive positions.
void initialize_cells_array(void);
