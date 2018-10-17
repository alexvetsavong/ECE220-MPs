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

	if (row == 0 && col == boardColSize){
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

	if (row == boardRowSize && col == 0){
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

	if (row == boardRowSize && col == boardColSize){
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

	if ((row == 0 || row == boardRowSize) && (col != 0 && col != boardColSize)){
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

		if (row == boardRowSize){
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

		if (col == boardColSize){
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
	int i, ret, newBoard[boardRowSize*boardColSize];
	for (i = 0; i < boardRowSize * boardColSize; i++){

		ret = countLiveNeighbor(board, boardRowSize, boardColSize, i/boardColSize, i%boardColSize);

		if (ret < 2){
			newBoard[i] = 0;
		}

		if (!board[i] && ret == 2){
			newBoard[i] = 0;
		}

		if ((board[i] != 0 && ret == 2) || ret == 3){
			newBoard[i] = 1;
		}


		if (ret > 3){
			newBoard[i] = 0;
		}
	}
	for (i = 0; i < boardRowSize * boardColSize; i++){
		board[i] = newBoard[i];
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
	int i, ret, life_check = 1, live_count, nextState[boardRowSize*boardColSize];
	for (i = 0; i < boardRowSize * boardColSize; i++){

		live_count = countLiveNeighbor(board, boardRowSize, boardColSize, i/boardColSize, i%boardColSize);

		if (live_count < 2){
			nextState[i] = 0;
		}

		if (!board[i] && live_count == 2){
			nextState[i] = 0;
		}

		if ((board[i] != 0 && live_count == 2) || live_count == 3){
			nextState[i] = 1;
			life_check = 0; // flag indicating that there is at least one live cell in nextState
		}


		if (live_count > 3){
			nextState[i] = 0;
		}
	}

	for (i=0; i < boardColSize*boardRowSize; i++){
		if (board[i] != nextState[i]){
			ret = 0;
			break;
		}
		ret = 1;
	}
if (!life_check) ret = 1;
return ret;
}
