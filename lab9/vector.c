#include "vector.h"
#include <stdlib.h>


vector_t * createVector(int initialSize)
{
    vector_t* vector_ptr;
    vector_ptr = (vector_t*)malloc(sizeof(vector_t));
    vector_ptr->maxSize = initialSize;
    vector_ptr->size = 0;
    vector_ptr->array = malloc(initialSize*sizeof(int));
    return vector_ptr;
}

void destroyVector(vector_t * vector)
{
  free(vector->array);
  free(vector);
}

void resize(vector_t * vector)
{
  realloc(vector->array, vector->maxSize * 2 * sizeof(int));
  vector->maxSize *= 2;
}

void push_back(vector_t * vector, int element)
{
  if (vector->size == vector->maxSize){
    resize(vector);
  }
  vector->array[vector->size] = element;
  vector->size++;
}

int pop_back(vector_t * vector)
{
  if(vector->size <= 0) return 0;
  vector->size--;
  return vector->array[vector->size];
}

int access(vector_t * vector, int index)
{
  if(index < 0 || index > vector->maxSize) return 0;
  return vector->array[index];
}
