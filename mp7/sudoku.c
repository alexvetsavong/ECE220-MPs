/* Name: Alex Vetsavong
 * NetID: avetsa2
 * Assignment: MP7: Solve Sudoku
 * Date: 10/24/2018
 *
 * This program will attempt to solve sudoku by recursively filling out a board  with
 * as many possible valid guesses possible, and checking to see if that board is
 * completely filled. It uses three functions checking each row, column, and zone,
 * and a helper function that tells whether a value is valid when entering a guess into a space.
 * If a board is filled, and it is not a solution, it will go back and try the next possible board.
 *
 */

#include "sudoku.h"
//-------------------------------------------------------------------------------------------------
// Start here to work on your MP7
//-------------------------------------------------------------------------------------------------

// You are free to declare any private functions if needed.

// Function: is_val_in_row
// Return true if "val" already existed in ith row of array sudoku.
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  // BEG TODO
  int j; // column index
  for (j=0; j < 9; j++){
    if(sudoku[i][j] == val) return 1; // check all values in the row to see if guess is in there already
  }
  return 0;
  // END TODO
}

// Function: is_val_in_col
// Return true if "val" already existed in jth column of array sudoku.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {

  assert(j>=0 && j<9);

  // BEG TODO
  int i; // row index
  for(i=0;i<9;i++){
    if(sudoku[i][j] == val) return 1; // check all values in the column to see if guess is in there already
  }
  return 0;
  // END TODO
}

// Function: is_val_in_3x3_zone
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  // BEG TODO
  int k, l; // iterator variables
  int a, b; // index variables
  a = (i/3)*3; // gets the starting row of corresponding 3x3 zone
  b = (j/3)*3; // gets first column of corresponding 3x3 zone

  // goes through each cell in 3x3 zone and sees if val is in the current zone
  for(k = a; k < a+3; k++){
    for(l = b; l < b+3; l++){
      if (sudoku[k][l]==val){ // if val is in the zone, return true
        return 1;
      }
    }
  }

  return 0;
  // END TODO
}

// Function: is_val_valid
// Return true if the val is can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

  assert(i>=0 && i<9 && j>=0 && j<9);

  // BEG TODO
  int check_row, check_col, check_zone;
  check_row = is_val_in_row(val, i, sudoku);
  check_col = is_val_in_col(val, j, sudoku);
  check_zone = is_val_in_3x3_zone(val, i, j, sudoku);
  if (check_row || check_col || check_zone) return 0;
  return 1;
  // END TODO
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) {

  // BEG TODO.
  int i,j; //iterator values
  int num; // number 1-9 that goes into cell
  int solution; // = 0 if board is not a solution, = 1 if it is a solution

  // check if all cells in board are filled
  for(i = 0; i < 9; i++){
    for(j = 0; j < 9; j++){
      if(!sudoku[i][j]) {
        solution = 0;
        break;
      }
    }
    if (solution == 0) break;
  }

  if (solution) return 1;

  // start at next empty cell
  for(i = 0; i < 9; i++){
    for(j = 0; j < 9; j++){
      if(!sudoku[i][j]){                       // if we are at the next empty cell
        for(num = 1; num <= 9; num++){         // start checking numbers 1-9
          if(is_val_valid(num, i, j, sudoku)){ // if the number is a valid entry
            sudoku[i][j] = num;                // place it in board
              if (solve_sudoku(sudoku))        // continue doing this until we have a full board
                return 1;                      // if the full board is a solution, return 1, cascading to nested functions
            sudoku[i][j] = 0;                  // if the full board is not a solution,
                                               // reset our previous guess, try again with new number
          }
        }
        return 0;                              // if the guesses in current cell all fail, return 0 and go back to previous cell
      }
    }
  }
  // END TODO.
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}
