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
	if (row == 0 && col == 0){
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

	if (row == 0 && col == boardColSize-1){
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

	if (row == boardRowSize-1 && col == 0){
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

	if (row == boardRowSize-1 && col == boardColSize-1){
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

	if ((row == 0 || row == boardRowSize-1) && (col != 0 && col != boardColSize)){
		if (row == 0){
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

		if (row == boardRowSize-1){
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

	if ((col == 0 || col == boardColSize) && (row !=0 && row != boardRowSize)){
		if (col == 0){
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

		if (col == boardColSize-1){
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
	int row, col, ret, nextState[boardRowSize*boardColSize];
	for (row = 0; row < boardRowSize; row++){
		for (col = 0; col < boardColSize; col++){
			ret = countLiveNeighbor(board, boardRowSize, boardColSize, row, col);

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
	stable_check = 1;
	for (row = 0; row < boardRowSize; row++){
		for (col = 0; col < boardColSize; col++){
			alive_count = countLiveNeighbor(board, boardRowSize, boardColSize, row, col);

			if (alive_count < 2){
				nextState[row*boardColSize+col] = 0;
			}

			if (alive_count == 2){
				if (!board[row*boardColSize+col]){
					nextState[row*boardColSize+col] = 0;
				}
				else{
					nextState[row*boardColSize+col] = 1;
					life_check = 1; // there is life in the board
				}
			}

			if (alive_count == 3){
				nextState[row*boardColSize+col] = 1;
				life_check = 1; // there is life in the board
			}

			if (alive_count > 3){
				nextState[row*boardColSize+col] = 0;
			}
		}
	}

	for (row=0;row<boardRowSize;row++){
		for (col=0;col<boardColSize;col++){
			if (board[row*boardColSize+col] != nextState[row*boardColSize+col]){
				stable_check = 0;
				ret = 0;
			}
		}
	}
if (!life_check || stable_check) ret = 1;
return ret;
}
