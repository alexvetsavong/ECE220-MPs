/*
* Name:       Alex Vetsavong
* NetID:      avetsa2
* Assignment: mp3.c
*
* This mp3 implements a factorial function which is used to
* calculate the coefficient of a row in Pascal's triangle through
* an iterative construct which calculates each coefficient in the
* row. This works for each row index entered up to the 12th row index,
* and prints out a single row to console.
*
*/

#include <stdio.h>
#include <stdlib.h>
int i, j, result, factorial(int parameter);
int main()
{
  int row;
  unsigned long coeff;
  printf("Enter a row index: ");
  scanf("%d",&row);
  printf("\n");
  // Write your code here
  for (j=0;j<=row;j++){
    coeff = factorial(row)/(factorial(j)*factorial(row-j));
    printf("%lu ", coeff);
    }

  printf("\n");
  return 0;
}

int factorial (int parameter){
  result = 1;                               /* initializes the factorial as 1*/
  if (parameter > 0){
    for (i=0; i < parameter; i++){          /* if the input is not 0, then use */
      result *= (parameter-i);              /* to find the factorial.*/
    }
  }
  else result = 1;                          /* if input is zero, return 1*/
  return result;
}
