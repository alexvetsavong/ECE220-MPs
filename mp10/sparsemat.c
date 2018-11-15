// Name: Alex Vetsavong
// NetID: avetsa2
// Date: 11/15/2018
//
// Assignment: MP10 - Sparse Matrix
//
// Description:
// This MP creates a sparse matrix consisting of non-zero tuples stored
// in memory as a list of tuples, represented by a linked list. The program parses
// a given text file for a formatted input, and reads the characters line-by-line,
// and creating nodes for values that have not been saved yet, and updating the
// values or deleting the nodes dependent on the inputs in the text file. The
// code implements a helper function, bubbleSort, which reorders the nodes in
// the linked lists by row-major order, and swapping the addreses of the node by
// referencing them as a pointer to a pointer.
//
// This eliminates the need to keep track of different special cases other than
// having to reattach the head node to the pointer in the sparse matrix struct.
//
// The code then proceeds to implement the rest of the included functions for
// saving the list of tuples, giving the value of a tuple given the row and
// column index, and freeing the matrices from memory.
//
// These functions are used in order to perform the matrices calculations between
// two sparse matrices.
//
//

#include "sparsemat.h"

#include <stdio.h>
#include <stdlib.h>

void bubbleSort(sp_tuples * mat_t){
  sp_tuples_node *head = mat_t->tuples_head;
  sp_tuples_node *temp = NULL;
  mat_t->tuples_head = NULL;

  while(head){
    sp_tuples_node **left = &head;
    sp_tuples_node **right = &(head->next);
    int swap_check = 0;

    while(*right){
      // if row-major index of the right node is less than the left, swap.
      if (((*right)->row < (*left)->row) || (((*right)->row == (*left)->row) && ((*right)->col < (*left)->col))){
        temp = *right;
        *right = *left; // swap the two nodes
        *left = temp;   // using the temp pointer to do so.

        temp = (*right)->next;          // now we swap their pointers.
        (*right)->next = (*left)->next; // doing the same thing
        (*left)->next = temp;

        // since we swapped pointers, we need to point the left pointer to the correct node.
        // (right is already in the correct location since we use that as our reference of comparison)
        left = &(*left)->next;
        swap_check++;
      }
      else{
        left = right;             // if we didn't swap
        right = &(*right)->next;  // we need to increment the left and right to the next pointer-to-pointers
      }
    }

    *right = mat_t->tuples_head;  // we reattach the last node to the sorted list

    if(swap_check){
      mat_t->tuples_head = *left; // if we swapped at any point, the last node gets pushed into the head
      *left = NULL;               // and we clear the node at the end off the list
    }
    else{
      mat_t->tuples_head = head;  // if there are no more swaps, reassign the head of the list to the struct
      break;
    }
  }
}

sp_tuples * load_tuples(char* input_file)
{
  sp_tuples * mat_t;
  mat_t = (sp_tuples *)malloc(sizeof(sp_tuples));             // allocate memory for the pointer to mat_t and its members

  sp_tuples_node *current = NULL, *next = NULL, *temp = NULL; // placeholder pointers to nodes for later use.

  int row, col, mat_rows, mat_cols;   // rows and cols variables to pass into struct
  double value;                       // value held in that row-column index
  char buf[128];                      // array to temporarily store values as string

  FILE * fp = fopen(input_file, "r"); // Open the file with the tuple inputs

  fseek(fp, 0, SEEK_SET);                     // move file page pointer to beginning of file
  fgets(buf,128,fp);                          // move to second line and scan first line as string into buffer char array
  sscanf(buf, "%d %d", &mat_rows, &mat_cols); // scan first line as formatted string into local variables

  mat_t->m = mat_rows;          // set parameter in struct to the number of rows in matrix
  mat_t->n = mat_cols;          // set parameter in struct to the number of cols in matrix
  mat_t->nz = 0;                // initialize count of non-zero entries to 0
  mat_t->tuples_head = NULL;    // initialize tuples_head to NULL, in case the matrix has no non-zero entries.

  while(1){
    fgets(buf,128,fp);
    if (feof(fp)) break;
    sscanf(buf, "%d %d %lf", &row, &col, &value); // scan first line as formatted string into local variables
    set_tuples(mat_t, row, col, value);           // check what to do to the linked list with the given tuple
  }
  bubbleSort(mat_t);

  fclose(fp); // Close the file
  return mat_t;
}

double gv_tuples(sp_tuples * mat_t,int row,int col)
{
  sp_tuples_node * current = mat_t->tuples_head;      // set a node to traverse linked list
  while(current != NULL){                             // keep going until we traverse off the linked list
    if (current->row == row && current->col == col) { // if we find a node with the same row-major index
      return current->value;                          // return the value in that node
    }
    current = current->next;                          // traverse to the next node
  }
  return 0;                                           // if there is no node there, return 0
}

void set_tuples(sp_tuples * mat_t, int row, int col, double value)
{
  sp_tuples_node * current = mat_t->tuples_head;
  sp_tuples_node * temp = NULL;
  if (value){
    temp = current;
    while (current != NULL){
      if (current->row == row && current->col == col){
        current->value = value;
        return;
      }
      current = current->next;
    }
    mat_t->tuples_head = (sp_tuples_node *)malloc(sizeof(sp_tuples_node));
    current = mat_t->tuples_head;
    current->row = row;
    current->col = col;
    current->value = value;
    current->next = temp;
    mat_t->nz++;
    return;
  }
  if (!value){
    if (current == NULL){
      return;
    }
    if (current != NULL){
      while (current->next != NULL){
        if (current->next->row == row && current->next->col == col){
          temp = current->next;
          current->next = current->next->next;
          free(temp);
          mat_t->nz--;
          return;
        }
        current = current->next;
      }
      if(current->next == NULL){
        if (current->row == row && current->col == col){
          free(current);
          mat_t->nz--;
        }
      }
    }
  }
  return;
}

void save_tuples(char * file_name, sp_tuples * mat_t)
{
  int row, col;
  double value;
  sp_tuples_node *current = mat_t->tuples_head; // start at the head of the list of tuples
  FILE * fp = fopen(file_name, "w");            // Open file_name for writing
  fprintf(fp, "%d %d\n", mat_t->m, mat_t->n);
  while (current != NULL){
    row = current->row;
    col = current->col;
    value = current->value;
    current = current->next;
    fprintf(fp, "%d %d %lf\n", row, col, value);
  }
  fclose(fp);
	return;
}

sp_tuples * add_tuples(sp_tuples * matA, sp_tuples * matB){
  int m1 = matA->m, m2 = matB->m, n1 = matA->n, n2 = matB->n;
  if (m1 != m2 || n1 != n2) return NULL;

  sp_tuples * retmat = (sp_tuples *)malloc(sizeof(sp_tuples));
  retmat->m = m1;
  retmat->n = n1;
  retmat->nz = 0;
  retmat->tuples_head = NULL;

	return retmat;
}

sp_tuples * mult_tuples(sp_tuples * matA, sp_tuples * matB){
  int m1 = matA->m, m2 = matB->m, n1 = matA->n, n2 = matB->n;
  if (n1 != m2) return NULL;

  sp_tuples * retmat = (sp_tuples *)malloc(sizeof(sp_tuples));
  retmat->m = m1;
  retmat->n = n2;
  retmat->nz = 0;
  retmat->tuples_head = NULL;

  return retmat;
}

void destroy_tuples(sp_tuples * mat_t){
  sp_tuples_node * temp = NULL;
  while(mat_t->tuples_head != NULL){
    temp = mat_t->tuples_head->next;
    free (mat_t->tuples_head);
    mat_t->tuples_head = temp;
  }
  free(mat_t);
  return;
}
