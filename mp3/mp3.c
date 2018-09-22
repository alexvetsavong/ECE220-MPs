/*
* Name:       Alex Vetsavong
* NetID:      avetsa2
* Assignment: mp3.c
*
* Iterative construct that performs the
* required calculations for each element in a given row.
* The loop will iterate through each row element and print it
* to screen until the end of the row has been reached, and the
* program is terminated. The program can handle row indexes up
* to the 40th index given that the result is stored as an
* unsigned long integer.
*
*/

#include <stdio.h>
#include <stdlib.h>
int main()
{
  int row;

  long double i;                       /* must be float value or else calculations*/
                                  /* will not be performed                   */
  unsigned long long result;
  printf("Enter a row index: ");
  scanf("%d",&row);
  printf("\n");
  /*Written code below*/
  result = 1;
  printf("%llu ", result);
  for (i=1; i <= row; i++){
    result *= ((row+1-i)/i);      /* given formula in the wikipage for the */
    printf("%llu ", result);      /* coefficient in any given row position.*/
  }                               /* It will print out a value after each  */
                                  /* calculation that is performed.        */
  printf("\n");
  return 0;
}
