/* Name: 	Alex Vetsavong
 * NetID: avetsa2
 * MP6: 	Conway's Game of Life
 * Date: 	10/18/2018
 *
 * This .c file implements the functions defined in updateBoard.h by using
 * pointers and arrays to iterate through each element in the array which represents
 * the game board.
 *
 * countLiveNeighbor will check all adjacent neighboring cells through conditionals.
 *
 * updateBoard calls countLiveNeighbor and updates each cell based on the return value
 * of countLiveNeighbor which is saved onto a new board. The new board (nextState) is
 * then stored into the array of board.
 *
 * aliveStable does the same thing as updateBoard, but compares the current board to
 * the nextState board in order to see if the board is stable or if the next state is
 * void of life.
*/

/*
 * countLiveNeighbor
 * Inputs:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * row: the row of the cell that needs to count alive neighbors.
 * col: the col of the cell that needs to count alive neighbors.
 * Output:
 * return the number of alive neighbors. There are at most eight neighbors.
 * Pay attention for the edge and corner cells, they have less neighbors.
 */

int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){
	int alive_count;
	alive_count = 0;
	/* This will check for whether or not the cell is a corner */
	if (row == 0 && col == 0){ // upper-left corner has three neighbors to the right and below
		if (board[(row*boardColSize+col)+1]){
			alive_count++;
		}
	      	if (board[(row*boardColSize+col)+boardColSize]){
			alive_count++;
		}
		if (board[(row*boardColSize+col)+boardColSize+1]){
			alive_count++;
		}
	}
	/* Another corner case */
	if (row == 0 && col == boardColSize-1){ // upper-right corner has three neighbors to the left and below
		if (board[(row*boardColSize+col)-1]){
			alive_count++;
		}
	      	if (board[(row*boardColSize+col)+boardColSize]){
			alive_count++;
		}
		if (board[(row*boardColSize+col)+boardColSize-1]){
			alive_count++;
		}
	}

	/* Another corner case */
	if (row == boardRowSize-1 && col == 0){ // bottom-left corner has three neighbors to the right and above
		if (board[(row*boardColSize+col)+1]){
			alive_count++;
		}
	      	if (board[(row*boardColSize+col)-boardColSize]){
			alive_count++;
		}
		if (board[(row*boardColSize+col)-boardColSize+1]){
			alive_count++;
		}
	}

	/* Another corner case */
	if (row == boardRowSize-1 && col == boardColSize-1){ // bottom-right corner has three neighbors to the left and above
		if (board[(row*boardColSize+col)-1]){
			alive_count++;
		}
	      	if (board[(row*boardColSize+col)-boardColSize]){
			alive_count++;
		}
		if (board[(row*boardColSize+col)-boardColSize-1]){
			alive_count++;
		}
	}

	/* this conditional checks if we are on the top row or bottom row and not on a corner */
	if ((row == 0 || row == boardRowSize-1) && (col != 0 && col != boardColSize)){
		if (row == 0){ // this is the top row of the board
			if (board[(row*boardColSize+col)-1]){
				alive_count++;
			}
			if (board[(row*boardColSize+col)+1]){
				alive_count++;
			}
			if (board[(row*boardColSize+col)+boardColSize-1]){
				alive_count++;
			}
			if (board[(row*boardColSize+col)+boardColSize]){
				alive_count++;
			}
			if (board[(row*boardColSize+col)+boardColSize+1]){
				alive_count++;
			}
		}

		if (row == boardRowSize-1){ // this is the bottom row of the board
			if (board[(row*boardColSize+col)-1]){
				alive_count++;
			}
			if (board[(row*boardColSize+col)+1]){
				alive_count++;
			}
			if (board[(row*boardColSize+col)-boardColSize-1]){
				alive_count++;
			}
			if (board[(row*boardColSize+col)-boardColSize]){
				alive_count++;
			}
			if (board[(row*boardColSize+col)-boardColSize+1]){
				alive_count++;
			}
		}
	}
	/* This checks for the left and right column cases not on a corner */
	if ((col == 0 || col == boardColSize) && (row !=0 && row != boardRowSize)){
		if (col == 0){ // This is the left-most column
			if (board[(row*boardColSize+col)-boardColSize]){
				alive_count++;
			}
			if (board[(row*boardColSize+col)-boardColSize+1]){
				alive_count++;
			}
			if (board[(row*boardColSize+col)+1]){
				alive_count++;
			}
			if (board[(row*boardColSize+col)+boardColSize]){
				alive_count++;
			}
			if (board[(row*boardColSize+col)+boardColSize+1]){
				alive_count++;
			}
		}

		if (col == boardColSize-1){ // This is the right-most column
			if (board[(row*boardColSize+col)-boardColSize]){
				alive_count++;
			}
			if (board[(row*boardColSize+col)-boardColSize-1]){
				alive_count++;
			}
			if (board[(row*boardColSize+col)-1]){
				alive_count++;
			}
			if (board[(row*boardColSize+col)+boardColSize]){
				alive_count++;
			}
			if (board[(row*boardColSize+col)+boardColSize-1]){
				alive_count++;
			}
		}
	}

	/* If it is not an edge or corner case, move to default case within the board */
	if (row != 0 && row != boardRowSize-1 && col != 0 && col !=boardColSize-1){
		if (board[(row*boardColSize+col)-boardColSize]){
			alive_count++;
		}
		if (board[(row*boardColSize+col)-boardColSize-1]){
			alive_count++;
		}
		if (board[(row*boardColSize+col)-boardColSize+1]){
			alive_count++;
		}
		if (board[(row*boardColSize+col)-1]){
			alive_count++;
		}
		if (board[(row*boardColSize+col)+1]){
			alive_count++;
		}
		if (board[(row*boardColSize+col)+boardColSize-1]){
			alive_count++;
		}
		if (board[(row*boardColSize+col)+boardColSize]){
			alive_count++;
		}
		if (board[(row*boardColSize+col)+boardColSize+1]){
			alive_count++;
		}
	}
	return alive_count;
}

/*
 * Update the game board to the next step.
 * Input:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: board is updated with new values for next step.
 */
void updateBoard(int* board, int boardRowSize, int boardColSize) {
	int row, col, ret;
	int nextState[boardRowSize*boardColSize]; /* initialize new board array for updating by step */
	for (row = 0; row < boardRowSize; row++){
		for (col = 0; col < boardColSize; col++){
			/* use ret value of countLiveNeighbor to check for the cases */
			ret = countLiveNeighbor(board, boardRowSize, boardColSize, row, col);

			/* these are the rules of the Game of Life */
			if (ret < 2){
				nextState[row*boardColSize+col] = 0;
			}

			if (ret == 2){
				if (!board[row*boardColSize+col]){
					nextState[row*boardColSize+col] = 0;
				}
				else nextState[row*boardColSize+col] = 1;
			}

			if (ret == 3){
				nextState[row*boardColSize+col] = 1;
			}

			if (ret > 3){
				nextState[row*boardColSize+col] = 0;
			}
		}
	}
	for (row = 0; row < boardRowSize; row++){
		for (col = 0; col < boardColSize; col++){
			board[row*boardColSize+col] = nextState[row*boardColSize+col];
		}
	}
}

/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with
 * current step or there is no alive cells at all.
 * return 0 if the alive cells change for the next step.
 */
int aliveStable(int* board, int boardRowSize, int boardColSize){
	int row, col, ret, stable_check, life_check = 0, alive_count, nextState[boardRowSize*boardColSize];
	stable_check = 1; /* this is a flag to indicate whether board is stable. Assume stable by default. */
	for (row = 0; row < boardRowSize; row++){
		for (col = 0; col < boardColSize; col++){
			alive_count = countLiveNeighbor(board, boardRowSize, boardColSize, row, col);
			/* recreate the next state board down here */
			if (alive_count < 2){
				nextState[row*boardColSize+col] = 0;
			}

			if (alive_count == 2){
				if (!board[row*boardColSize+col]){
					nextState[row*boardColSize+col] = 0;
				}
				else{
					nextState[row*boardColSize+col] = 1;
					life_check = 1; /* Checks flag that there is life on the board */
				}
			}

			if (alive_count == 3){
				nextState[row*boardColSize+col] = 1;
				life_check = 1; 	/* there is life in the board */
			}

			if (alive_count > 3){
				nextState[row*boardColSize+col] = 0;
			}
		}
	}

	/* iterate through the next state board and current board, and compare each element to each other */
	for (row=0;row<boardRowSize;row++){
		for (col=0;col<boardColSize;col++){
			if (board[row*boardColSize+col] != nextState[row*boardColSize+col]){
				stable_check = 0; /* if they are not equal, set stability flag to false */
				ret = 0; 					/* set ret to 0 to show that board should change */
			}
		}
	}
if (!life_check || stable_check) ret = 1; /* if there is no life on the board or it is stable */
return ret;																/* return the value 1																*/
}
