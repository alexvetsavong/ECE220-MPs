/* Name: Alex Vetsavong
 * NetID: avetsa2
 * Date: 11/01/2018
 * Assignment: MP8 - 2048 Game
 *
 * This program is the implementation of the 2048 game which uses multiple functions to
 * determine the behavior of sliding tiles when a key is pressed to determine the direction
 * of the slide. This is determined by checking for multiple conditions during the shifting
 * of each element in the column and row, and as cells are meged, their combined value is added
 * to the total game score. This is done by utilizing structs and their characteristics in order to
 * pass multiple values and parameters between different functions. At the end, a temporary copy
 * of the game board is created in order to check whether or not the game has reached an happened
 * when no further moves can be made.
 *
 */

#include "game.h"

game * make_game(int rows, int cols)
/*! Create an instance of a game structure with the given number of rows
    and columns, initializing elements to -1 and return a pointer
    to it. (See game.h for the specification for the game data structure)
    The needed memory should be dynamically allocated with the malloc family
    of functions.
*/
{
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    game * mygame = malloc(sizeof(game));
    mygame->cells = malloc(rows*cols*sizeof(cell));

    //YOUR CODE STARTS HERE:  Initialize all other variables in game struct
    int i;
    mygame->rows = rows;
    mygame->cols = cols;
    cell * cells_ptr = mygame->cells;
    for(i=0;i<rows*cols;i++){
      *cells_ptr = -1;
      cells_ptr++;
    }
    mygame->score = 0;
    return mygame;
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any
	memory previously allocated is not lost in this function.
*/
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

	//YOUR CODE STARTS HERE:  Re-initialize all other variables in game struct
  int i;
  (*_cur_game_ptr)->rows = new_rows;
  (*_cur_game_ptr)->cols = new_cols;
  cell * cells_ptr = (*_cur_game_ptr)->cells;
  for(i=0;i<new_rows*new_cols;i++){
    *cells_ptr = -1;
    cells_ptr++;
  }
  (*_cur_game_ptr)->score = 0;
	return;
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
    //YOUR CODE STARTS HERE
    cell * cell_ptr;
    if(row<(cur_game->rows) && col< (cur_game->cols)){
      cell_ptr = &(cur_game->cells[row*(cur_game->cols)+col]);
      return cell_ptr;
    }
    return NULL;
}

//These next four functions slide all columns/rows in the respective direction and merges in said direction
//The reason this all works is because we're sliding rows/columns before dealing with merges.
//This allows us to do merging by checking only "adjacent spaces" instead of having to look at all elements
//Like, as an example: 2 4 ** 4 when you press 'A' will first do 2 4 4 ** and then try to handle merges.

int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the
   one above it, the tiles are merged by adding their values together. When
   tiles merge, increase the score by the value of the new tile. A tile can
   not merge twice in one turn. If sliding the tiles up does not cause any
   cell to change value, w is an invalid move and return 0. Otherwise, return 1.
*/
{
    //YOUR CODE STARTS HERE
    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i, j, k;
    int moves_cntr = 0, merge_check;

    for (j = 0; j < cols; j++) {
      merge_check = rows;           // initialize this check to a value off of the board, so we don't use it by mistake.
      for (i = 1; i < rows; i++) {  // traverse the column by going down rows.
        if (*get_cell(cur_game, i, j) != -1) {
          for (k = 0; k < i; k++) {
            if (*get_cell(cur_game, k, j) == -1) {
              break;
            }
          }
          if (k < i) {                                             // if the target row is a row that is higher than the current row
            *get_cell(cur_game, k, j) = *get_cell(cur_game, i, j); // Move current tile to target row
            *get_cell(cur_game, i, j) = -1;                        // Clear the current row's tile
            moves_cntr++;                                          // keep track of our moves
            if ((k*cols + j) != j) {                               // If k != 0, then we check the target row and the row above it
              if (*get_cell(cur_game, k, j) == *get_cell(cur_game, k-1, j)) { // If they're equal, we merge the two
                if ((k-1) != merge_check) {                                   // check to see if we have merged to this location before
                  merge_check = k-1;                                          // if not, say that we have and continue merging
                  *get_cell(cur_game, k-1, j) += *get_cell(cur_game, k, j);   // Merge the two cells
                  cur_game->score += *get_cell(cur_game, k-1, j);             // add merged value to the game score.
                  *get_cell(cur_game, k, j) = -1;                             // Clear the target row that we are on and move on.
                }
              }
            }
          }
          else {                                                           // If there are no gaps going down the columns, we attempt to merge adjacent cells.
            if(*get_cell(cur_game, i, j) == *get_cell(cur_game, i-1, j)) { // This is the same logic as a few lines above but for no empty spaces
              if ((i-1) != merge_check) {                                  // Do another check for previous merge at location
                merge_check = i-1;                                         // If not, say it will be merged
                *get_cell(cur_game, i-1, j) += *get_cell(cur_game, i, j);  // Merge the tiles
                cur_game->score += *get_cell(cur_game, i-1, j);            // Add merged value to score total.
                *get_cell(cur_game, i, j) = -1;                            // Clear the current tile after merging with the above.
                moves_cntr++;                                              // Keep track of the number of moves made.
              }
            }
          }
        }
      }
    }
    if (moves_cntr == 0) { // Chceck to see if any moves have been made
      return 0;
    }
    return 1;              // Otherwise, it is a legal slide
};

// All of the functions for moving the tiles are implemented in the same way, but
// just done by changing iterative variables and direction of increment/decrement.
// Look to them for any changes.

int move_s(game * cur_game)
{
    //YOUR CODE STARTS HERE
    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i, j, k;
    int moves_cntr = 0, merge_check;

    for (j = 0; j < cols; j++) {
      merge_check = rows;
      for (i = (rows-2); i >= 0; i--) {
        if (*get_cell(cur_game, i, j) != -1) {
          for (k = (rows-1); k > i; k--) {
            if (*get_cell(cur_game, k, j) == -1) {
              break;
            }
          }
          if (k > i) {
            *get_cell(cur_game, k, j) = *get_cell(cur_game, i, j);
            *get_cell(cur_game, i, j) = -1;
            moves_cntr++;
            if ((k*cols + j) != ((rows-1)*cols + j)) {
              if (*get_cell(cur_game, k, j) == *get_cell(cur_game, k+1, j)) {
                if ((k+1) != merge_check) {
                  merge_check = k+1;
                  *get_cell(cur_game, k+1, j) += *get_cell(cur_game, k, j);
                  cur_game->score += *get_cell(cur_game, k+1, j);
                  *get_cell(cur_game, k, j) = -1;
                }
              }
            }
          }
          else {
            if(*get_cell(cur_game, i, j) == *get_cell(cur_game, i+1, j)) {
              if ((i+1) != merge_check) {
                merge_check = i+1;
                *get_cell(cur_game, i+1, j) += *get_cell(cur_game, i, j);
                cur_game->score += *get_cell(cur_game, i+1, j);
                *get_cell(cur_game, i, j) = -1;
                moves_cntr++;
              }
            }
          }
        }
      }
    }
    if (moves_cntr == 0) {
      return 0;
    }
    return 1;
};

int move_a(game * cur_game)
{
    //YOUR CODE STARTS HERE
    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i, j, k;
    int moves_cntr = 0, merge_check;

    for (i = 0; i < rows; i++) {
      merge_check = rows;
      for (j = 1; j < cols; j++) {
        if (*get_cell(cur_game, i, j) != -1) {
          for (k = 0; k < j; k++) {
            if (*get_cell(cur_game, i, k) == -1) {
              break;
            }
          }
          if (k < j) {
            *get_cell(cur_game, i, k) = *get_cell(cur_game, i, j);
            *get_cell(cur_game, i, j) = -1;
            moves_cntr++;
            if (k != 0) {
              if (*get_cell(cur_game, i, k) == *get_cell(cur_game, i, k-1)) {
                if ((k-1) != merge_check) {
                  merge_check = k-1;
                  *get_cell(cur_game, i, k-1) += *get_cell(cur_game, i, k);
                  cur_game->score += *get_cell(cur_game, i, k-1);
                  *get_cell(cur_game, i, k) = -1;
                }
              }
            }
          }
          else {
            if(*get_cell(cur_game, i, j) == *get_cell(cur_game, i, j-1)) {
              if ((j-1) != merge_check) {
                merge_check = j-1;
                *get_cell(cur_game, i, j-1) += *get_cell(cur_game, i, j);
                cur_game->score += *get_cell(cur_game, i, j-1);
                *get_cell(cur_game, i, j) = -1;
                moves_cntr++;
              }
            }
          }
        }
      }
    }
    if (moves_cntr == 0) {
      return 0;
    }
    return 1;
};

int move_d(game * cur_game){
    //YOUR CODE STARTS HERE
    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i, j, k;
    int moves_cntr = 0, merge_check;

    for (i = 0; i < rows; i++) {
      merge_check = cols;
      for (j = (cols-2); j >= 0; j--) {
        if (*get_cell(cur_game, i, j) != -1) {
          for (k = (cols-1); k > j; k--) {
            if (*get_cell(cur_game, i, k) == -1) {
              break;
            }
          }
          if (k > j) {
            *get_cell(cur_game, i, k) = *get_cell(cur_game, i, j);
            *get_cell(cur_game, i, j) = -1;
            moves_cntr++;
            if (k != (cols-1)) {
              if (*get_cell(cur_game, i, k) == *get_cell(cur_game, i, k+1)) {
                if ((k+1) != merge_check) {
                  merge_check = k+1;
                  *get_cell(cur_game, i, k+1) += *get_cell(cur_game, i, k);
                  cur_game->score += *get_cell(cur_game, i, k+1);
                  *get_cell(cur_game, i, k) = -1;
                }
              }
            }
          }
          else {
            if(*get_cell(cur_game, i, j) == *get_cell(cur_game, i, j+1)) {
              if ((j+1) != merge_check) {
                merge_check = j+1;
                *get_cell(cur_game, i, j+1) += *get_cell(cur_game, i, j);
                cur_game->score += *get_cell(cur_game, i, j+1);
                *get_cell(cur_game, i, j) = -1;
                moves_cntr++;
              }
            }
          }
        }
      }
    }
    if (moves_cntr == 0) {
      return 0;
    }
    return 1;
};

int legal_move_check(game * cur_game)
/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
    //YOUR CODE STARTS HERE

    // This code will make a new copy of the game board so that we can check if there are any more legal moves
    // without changing the state of the current board.
    game checkgame;               //new struct for a temp game
    game * tmp = &checkgame;      //struct pointer to checkgame
    int i, j;
    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int score = cur_game->score;
    int tmpcells[rows*cols];      //tmp array to store copied cells

    for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++) {
        tmpcells[i*cols+j] = cur_game->cells[i*cols+j]; // copies values from our board to the temporary board
      }
    }

    tmp->rows = rows;
    tmp->cols = cols;
    tmp->score = score;
    tmp->cells = tmpcells;
    //If moving in any direction will return 0, no moves are allowed.
    if ( (move_w(tmp)==0) && (move_a(tmp)==0) && (move_s(tmp)==0) && (move_d(tmp)==0) ) {
      return 0;
    }
    return 1;
}


/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{

	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    if (cell_ptr == NULL){
        printf("Bad Cell Pointer.\n");
        exit(0);
    }


	//check for an empty cell
	int emptycheck = 0;
	int i;

	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}

    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game)
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;

	printf("\n\n\nscore:%d\n",cur_game->score);


	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char


    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  ");
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }

	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char

    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;

    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}

		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		}

		remake_game(&cur_game,rows,cols);

		move_success = 1;

    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }




    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game);
		 print_game(cur_game);
    }

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}
