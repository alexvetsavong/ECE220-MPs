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
  sp_tuples_node * current = mat_t->tuples_head;     // set the current node we are interested in to be the head
  sp_tuples_node * temp = NULL;                      // temporary pointer, for swapping and moving
  if (value){                                        // if we have a positive value in the tuple
    temp = current;                                  // set temp to the current head
    while (current != NULL){              // if it is not NULL (something is there), we can assign the value to that node.
      if (current->row == row && current->col == col){
        current->value = value;
        return;                           // once we assign the value, we return to caller.
      }
      current = current->next;            // if we didn't find it yet move to the next node, as long as it exists.
    }
    mat_t->tuples_head = (sp_tuples_node *)malloc(sizeof(sp_tuples_node));  // otherwise, we need to allocate a new node and create one
    current = mat_t->tuples_head; // set current node to the newly created node (for readability)
    current->row = row;           // these next four lines assign the necessary parameters to the new node
    current->col = col;
    current->value = value;
    current->next = temp;         // have this point back towards the older node
    mat_t->nz++;                  // increase the count for the number of non-zeroes
    return;
  }
  if (!value){                    // if the value is zero, we need to delete it
    if (current == NULL){         // if the head is null, we have no nodes, so just return
      return;
    }
    if (current != NULL){         // if the head is not null, traverse list to find the correct node to delete
      while (current->next != NULL){
        if (current->next->row == row && current->next->col == col){
          temp = current->next;                 //keep this safe so we can delete it
          current->next = current->next->next;  // reassign the next node to the one after the node pending oblivion (deletion)
          free(temp);                           // Kill it with C's equivalent of hell fire
          mat_t->nz--;                          // We have one less non-zero ally in the matrix
          return;
        }
        current = current->next;
      }
      if(current->next == NULL){  // If none of the above apply, and this node matches the one being deleted
        if (current->row == row && current->col == col){
          free(current);          // delete it
          mat_t->nz--;            // same reason as above
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
  fprintf(fp, "%d %d\n", mat_t->m, mat_t->n);   // Prints the dimensions of the matrix
  while (current != NULL){  // do this for every node
    row = current->row;     // copy all members into local variables
    col = current->col;
    value = current->value;
    current = current->next;
    fprintf(fp, "%d %d %lf\n", row, col, value);  // print the variables into file stream
  }
  fclose(fp);
	return;
}

sp_tuples * add_tuples(sp_tuples * matA, sp_tuples * matB){
  int m1 = matA->m, m2 = matB->m, n1 = matA->n, n2 = matB->n; // size of matrices
  int add_check = 0;
  double matC_value;
  if (m1 != m2 || n1 != n2) return NULL;

  // sp_tuples_node *c_mat_current = NULL;
  sp_tuples_node *temp = NULL;
  sp_tuples_node *matA_current = NULL;
  sp_tuples_node *matB_current = NULL;
  sp_tuples * matC = (sp_tuples *)malloc(sizeof(sp_tuples));

  matC->m = m1;
  matC->n = n1;
  matC->nz = 0;
  matC->tuples_head = NULL;

  // c_mat_current = matC->tuples_head;
  matA_current = matA->tuples_head;
  matB_current = matB->tuples_head;

  // We created nodes in matC for every value that is in matrix A
  while(matA_current != NULL){
    matC_value = gv_tuples(matA,matA_current->row,matA_current->col);
    set_tuples(matC,matA_current->row,matA_current->col,matC_value);
    matA_current = matA_current->next;
  }

  // We create nodes in matC for every new tuple in matrix B,
  // and if one already exists, we add the value from matrix B to matC
  while(matB_current != NULL){
    add_check = 0;
    temp = matC->tuples_head;   // assign pointer to tuples_head each time for checking for existing nodes and values
    while(temp != NULL){        // go through every node already in matC
      if(matB_current->row == temp->row && matB_current->col == temp->col){   // if one exists
        matC_value = temp->value;                                             // place the value from matC in a local variable
        matC_value += gv_tuples(matB, matB_current->row, matB_current->col);  // add the value of matrix B's element to it
        set_tuples(matC, temp->row, temp->col, matC_value);                   // update the value in matC or delete it if result is zero
        add_check++;                                                          // if we added, set this check to true
      }
      temp = temp->next;                                                      // go onto the next element in matC
    }
    if(!add_check) set_tuples(matC, matB_current->row, matB_current->col,matB_current->value); // if we did not add to any of the values, we need to create a new node
    matB_current = matB_current->next;                                                         // go on to next element in matrix B
  }

  bubbleSort(matC); // sort matC before returning it
	return matC;
}

sp_tuples * mult_tuples(sp_tuples * matA, sp_tuples * matB){
  int m1 = matA->m, m2 = matB->m, n1 = matA->n, n2 = matB->n; // sizes of matA and matB
  int mult_check = 0;         // flag for checking when we do a multiplication
  double matC_value = 0;      // accumulator for the value that goes into matC
  if (n1 != m2) return NULL;  // if the multiplication can't be done, return NULL

  sp_tuples * matC = (sp_tuples *)malloc(sizeof(sp_tuples));  // Initialize matC, the result matrix
  matC->m = m1;
  matC->n = n2;
  matC->nz = 0;
  matC->tuples_head = NULL;

  sp_tuples_node * matA_current = NULL; // use these for traversing the linked lists
  sp_tuples_node * matB_current = NULL; //
  sp_tuples_node * matC_current = NULL; //

  matA_current = matA->tuples_head;

  while(matA_current != NULL){          // start at the first non-zero element in matA
    matB_current = matB->tuples_head;   // point to the first non-zero in matB
    while(matB_current != NULL){
      matC_value = 0;                   // initalize accumulator at zero
      mult_check = 0;                   // flag for whether or not we need to create a new node
      matC_current = matC->tuples_head; // point to the tuples in matC
      if (matB_current->row == matA_current->col){  // if the row of the element in matB matches the column of element in matA
        matC_value += gv_tuples(matA, matA_current->row, matA_current->col)*gv_tuples(matB, matB_current->row, matB_current->col);  //multiply A by that value
        while(matC_current != NULL){  // check all the current nodes in matC
          if (matA_current->row == matC_current->row && matB_current->col == matC_current->col){  // if we find a matching one that corresponds to matA and matB
            matC_current->value += matC_value;  // add the accumulator into that element
            mult_check++;                       // toggle the flag for node creation
          }
          matC_current = matC_current->next;    // go through all nodes in matC
        }
        if (!mult_check) set_tuples(matC, matA_current->row, matB_current->col, matC_value);  // if no node exists, we make a new one
      }
      matB_current = matB_current->next;  // go through all nodes in matB
    }
    matA_current = matA_current->next;    // go through all nodes in matA
  }

  bubbleSort(matC); // sort matC before returning it
  return matC;
}

void destroy_tuples(sp_tuples * mat_t){
  sp_tuples_node * temp = NULL;
  while(mat_t->tuples_head != NULL){    // starts at the head and free's every node by traversing the linked list.
    temp = mat_t->tuples_head->next;    // safe-keeping
    free (mat_t->tuples_head);          // free the head
    mat_t->tuples_head = temp;          // assign the next node as the new head
  }
  free(mat_t);                          // when all nodes are free, free struct
  return;
}
