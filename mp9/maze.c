/* Name: Alex Vetsavong
 * NetID: avetsa2
 * Assignment: MP9: Maze Solver
 * Date: 10/7/2018
 *
 * Partners: No one, I have only myself to thank.
 *
 * Description: This program parses a given text file input of a certain format
 * which defines the width, height, and layout of the maze within the code.
 * Using the <stdio.h> C library, we parse the maze's dimensions using fscanf
 * and copying the layout of the text file, line by line, each as a single string,
 * into the corresponding row of the maze, whose first row is pointed to by a
 * double pointer stored as a member in a struct of type maze_t, which acts as a struct.
 *
 * Memory is allocated and freed as necessary for each cell in memory, and their pointers.
 *
 * The main body of the code deals with the edge and corner cases separate from the cells
 * which are within the maze, not on the borders of it. It also deals with the situation
 * that the position of the cell may be outside of the maze.
 *
 * It traverses the maze by first going right then down, left, or up in that proceeding
 * order. It does so by calling itself recursively if those tiles are empty. If it does not
 * reach a solution to the maze, it will backtrack all the way back to where it can make another
 * choice and attempt to travel down that path.
 *
 * This is done until the program reaches the end of the maze.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
    // Your code here. Make sure to replace following line with your own code.
    FILE *fp = fopen(fileName, "r");        // Open the file given the fileName char array
    maze_t * maze;                          // Create the struct
    maze = (maze_t*)malloc(sizeof(maze_t)); // Allocate space for the pointer to the struct and its members

    int w, h;       // Used to initialize the struct
    int i, j;       // iterator variables for rows and columns
    char BUF[128];  // Buffer array

    fscanf(fp,"%d", &w);  // Scan for the first integer in the file and save it as the width
    fscanf(fp,"%d", &h);  // Scan for the second integer and save it as the height

    maze->width = w;
    maze->height = h;
    maze->cells = (char **)malloc(h*sizeof(char **)); // Allocate space for this pointer to the beginning of the 2D array

    for (i=0; i < h; i++){
      maze->cells[i] = malloc(w*sizeof(char));        // Allocate columns to each row pointer
    }

    fseek(fp,0,SEEK_SET);               // Move back to start of the file
    fgets(BUF,128,fp);                  // This skips the top row which holds the width and height of the maze.

    for(i=0;i<maze->height;i++){
      fgets(BUF,128,fp);                // Get the current row of the maze
      for(j=0;j<maze->width;j++){
        maze->cells[i][j] = BUF[j];     // Copy current row to the struct
        if(maze->cells[i][j] == START){ // If the current cell is the start, save its location
          maze->startRow = i;
          maze->startColumn = j;
        }
        if(maze->cells[i][j] == END){   // If the current cell is the end, save its location
          maze->endRow = i;
          maze->endColumn = j;
        }
      }
    }

    fclose(fp);
    return maze;
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    // Your code here.
    int i;
    for(i=0; i < maze->height; i++){
      free(maze->cells[i]);           // Free the memory of each row and its columns
    }
    free(maze->cells);                // Free the memory of the pointer to the first row pointer
    free(maze);                       // Free the menory taken up by the struct and its members.
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
    // Your code here.
    int i, j;
    for(i=0;i<maze->height;i++){
      for(j=0;j<maze->width;j++){
        printf("%c", maze->cells[i][j]); // Print each character in the maze stored in the array within maze_t struct.
      }
      printf("\n");                      // Since we used fgets, we must print a new line ourselves.
    }
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */
int solveMazeDFS(maze_t * maze, int col, int row)
{
    // Your code here. Make sure to replace following line with your own code.

    // We have only one base case since we check if the next movement leads us to the end in each edge and default case
    if ((col < 0 || col >= maze->width) || (row < 0 || row >= maze->height) || maze->cells[row][col] == WALL){
      return 0; // Return this if the current location is invalid
    }

    // We will traverse the maze by going right, down, left, or up, in that order.
    // If the current location is at the corners
    if (col == 0 && row == 0){             // Upper-left corner -
                                           // explanation for code will be place only here. Other cases follow the same algorithm but for different directios.
      if (maze->cells[row][col+1] == END){ // We check to the right to see if we've already reached the end
        return 1;                          // Return true if that next move would lead us to the end
      }
      if (maze->cells[row][col+1] == EMPTY){ // If the row to the right is empty and not visited, we go that way.
        maze->cells[row][col+1] = PATH;      // Mark it as a solution path
        if (solveMazeDFS(maze, col+1, row)) return 1; // If this path leads us to the exit, return true
        else {
          maze->cells[row][col+1] = VISITED;          // Otherwise, mark it as visited and move on.
        }
      }
      if (maze->cells[row+1][col] == END){            // If moving down leads into the exit, return true
        return 1;
      }
      if (maze->cells[row+1][col] == EMPTY){          // If moving down leads into an empty space,
        maze->cells[row+1][col] = PATH;               // Mark it as part of the path
        if (solveMazeDFS(maze, col, row+1)) return 1; // Return true if it eventually leads to a solution path
        else {
          maze->cells[row+1][col] = VISITED;          // Otherwise, mark it as visited and move on.
        }
      }
    }
    if (col == maze->width - 1 && row == 0){ // Upper-right corner
      if (maze->cells[row+1][col] == END){
        return 1;
      }
      if (maze->cells[row+1][col] == EMPTY){
        maze->cells[row+1][col] = PATH;
        if (solveMazeDFS(maze, col, row+1)) return 1;
        else {
          maze->cells[row+1][col] = VISITED;
        }
      }
      if (maze->cells[row][col-1] == END){
        return 1;
      }
      if (maze->cells[row][col-1] == EMPTY){
        maze->cells[row][col-1] = PATH;
        if (solveMazeDFS(maze, col-1, row)) return 1;
        else {
          maze->cells[row][col-1] = VISITED;
        }
      }
    }
    if (col == 0 && row == maze->height - 1){ // Lower-left corner
      if (maze->cells[row][col+1] == END){
        return 1;
      }
      if (maze->cells[row][col+1] == EMPTY){
        maze->cells[row][col+1] = PATH;
        if (solveMazeDFS(maze, col+1, row)) return 1;
        else {
          maze->cells[row][col+1] = VISITED;
        }
      }
      if (maze->cells[row-1][col] == END){
        return 1;
      }
      if (maze->cells[row-1][col] == EMPTY){
        maze->cells[row-1][col] = PATH;
        if (solveMazeDFS(maze, col, row-1)) return 1;
        else {
          maze->cells[row-1][col] = VISITED;
        }
      }
    }
    if (col == maze->width - 1 && row == maze->height - 1){ // Lower-right corner
      if (maze->cells[row][col-1] == END){
        return 1;
      }
      if (maze->cells[row][col-1] == EMPTY){
        maze->cells[row][col-1] = PATH;
        if (solveMazeDFS(maze, col-1, row)) return 1;
        else {
          maze->cells[row][col-1] = VISITED;
        }
      }
      if (maze->cells[row-1][col] == END){
        return 1;
      }
      if (maze->cells[row-1][col] == EMPTY){
        maze->cells[row-1][col] = PATH;
        if (solveMazeDFS(maze, col, row-1)) return 1;
        else {
          maze->cells[row-1][col] = VISITED;
        }
      }
    }

    // If the current location is on an edge
    if (col == 0 && row != 0 && row != maze->height - 1){ // left edge
      if (maze->cells[row][col+1] == END){
        return 1;
      }
      if (maze->cells[row][col+1] == EMPTY){
        maze->cells[row][col+1] = PATH;
        if (solveMazeDFS(maze, col+1, row)) return 1;
        else {
          maze->cells[row][col+1] = VISITED;
        }
      }
      if (maze->cells[row+1][col] == END){
        return 1;
      }
      if (maze->cells[row+1][col] == EMPTY){
        maze->cells[row+1][col] = PATH;
        if (solveMazeDFS(maze, col, row+1)) return 1;
        else {
          maze->cells[row+1][col] = VISITED;
        }
      }
      if (maze->cells[row-1][col] == END){
        return 1;
      }
      if (maze->cells[row-1][col] == EMPTY){
        maze->cells[row-1][col] = PATH;
        if (solveMazeDFS(maze, col, row-1)) return 1;
        else {
          maze->cells[row-1][col] = VISITED;
        }
      }
    }
    if (col == maze->width - 1 && row != 0 && row != maze->height - 1){ // right edge
      if (maze->cells[row+1][col] == END){
        return 1;
      }
      if (maze->cells[row+1][col] == EMPTY){
        maze->cells[row+1][col] = PATH;
        if (solveMazeDFS(maze, col, row+1)) return 1;
        else {
          maze->cells[row+1][col] = VISITED;
        }
      }
      if (maze->cells[row][col-1] == END){
        return 1;
      }
      if (maze->cells[row][col-1] == EMPTY){
        maze->cells[row][col-1] = PATH;
        if (solveMazeDFS(maze, col-1, row)) return 1;
        else {
          maze->cells[row][col-1] = VISITED;
        }
      }
      if (maze->cells[row-1][col] == END){
        return 1;
      }    // If there are no empty cells around
      if (maze->cells[row-1][col] == EMPTY){
        maze->cells[row-1][col] = PATH;
        if (solveMazeDFS(maze, col, row-1)) return 1;
        else {
          maze->cells[row-1][col] = VISITED;
        }
      }
    }
    if (row == 0 && col != 0 && col != maze->width - 1){ // top edge
      if (maze->cells[row][col+1] == END){
        return 1;
      }
      if (maze->cells[row][col+1] == EMPTY){
        maze->cells[row][col+1] = PATH;
        if (solveMazeDFS(maze, col+1, row)) return 1;
        else {
          maze->cells[row][col+1] = VISITED;
        }
      }
      if (maze->cells[row+1][col] == END){
        return 1;
      }
      if (maze->cells[row+1][col] == EMPTY){
        maze->cells[row+1][col] = PATH;
        if (solveMazeDFS(maze, col, row+1)) return 1;
        else {
          maze->cells[row+1][col] = VISITED;
        }
      }
      if (maze->cells[row][col-1] == END){
        return 1;
      }
      if (maze->cells[row][col-1] == EMPTY){
        maze->cells[row][col-1] = PATH;
        if (solveMazeDFS(maze, col-1, row)) return 1;
        else {
          maze->cells[row][col-1] = VISITED;
        }
      }
    }
    if (row == maze->height - 1 && col != 0 && col!= maze->width - 1){ // bottom edge
      if (maze->cells[row+1][col] == END){
        return 1;
      }
      if (maze->cells[row+1][col] == EMPTY){
        maze->cells[row+1][col] = PATH;
        if (solveMazeDFS(maze, col, row+1)) return 1;
        else {
          maze->cells[row+1][col] = VISITED;
        }
      }
      if (maze->cells[row][col-1] == END){
        return 1;
      }
      if (maze->cells[row][col-1] == EMPTY){
        maze->cells[row][col-1] = PATH;
        if (solveMazeDFS(maze, col-1, row)) return 1;
        else {
          maze->cells[row][col-1] = VISITED;
        }
      }
      if (maze->cells[row-1][col] == END){
        return 1;
      }
      if (maze->cells[row-1][col] == EMPTY){
        maze->cells[row-1][col] = PATH;
        if (solveMazeDFS(maze, col, row-1)) return 1;
        else {
          maze->cells[row-1][col] = VISITED;
        }
      }
    }

    // If we're within the maze, away from the edges.
    if (row > 0 && row < maze->height - 1 && col > 0 && col < maze->width){ // not an edge case (can look in each direction)
      // start checking the cells to the right, down, left, and up
      if (maze->cells[row][col+1] == END){
        return 1;
      }
      if (maze->cells[row][col+1] == EMPTY){
        maze->cells[row][col+1] = PATH;
        if (solveMazeDFS(maze, col+1, row)) return 1;
        else {
          maze->cells[row][col+1] = VISITED;
        }
      }
      if (maze->cells[row+1][col] == END){
        return 1;
      }
      if (maze->cells[row+1][col] == EMPTY){
        maze->cells[row+1][col] = PATH;
        if (solveMazeDFS(maze, col, row+1)) return 1;
        else {
          maze->cells[row+1][col] = VISITED;
        }
      }
      if (maze->cells[row][col-1] == END){
        return 1;
      }
      if (maze->cells[row][col-1] == EMPTY){
        maze->cells[row][col-1] = PATH;
        if (solveMazeDFS(maze, col-1, row)) return 1;
        else {
          maze->cells[row][col-1] = VISITED;
        }
      }
      if (maze->cells[row-1][col] == END){
        return 1;
      }
      if (maze->cells[row-1][col] == EMPTY){
        maze->cells[row-1][col] = PATH;
        if (solveMazeDFS(maze, col, row-1)) return 1;
        else {
          maze->cells[row-1][col] = VISITED;
        }
      }
    }
    return 0;
}
