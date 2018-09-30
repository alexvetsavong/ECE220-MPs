/* Name:   Alex Vetsavong
*  NetID:  avetsa2
*  Date:   10/0/2018
*
*  MP4.c
*  This program checks for semiprimes by looking at each number in a range from
*  a to b which are given by the user. The program is guaranteed to work for
*  1 < a,b <= 200.
*
*  The program follows the algorithm provided on the wikipage, where each number
*  in the range of a and b has its factors looked at, and if its factor is prime
*  and the quotient of the number and the prime factor is also a prime number,
*  then the program will print that number to console. This is done by itera-
*  tively looping through the range of numbers provided and checking each number,
*  and each possibly factor, while using another function to check for primality.
*
*/

#include <stdlib.h>
#include <stdio.h>


int is_prime(int number);
int print_semiprimes(int a, int b);


int main(){
   int a, b;
   printf("Input two numbers: ");
   scanf("%d %d", &a, &b);
   if( a <= 0 || b <= 0 ){
     printf("Inputs should be positive integers\n");
     return 1;
   }

   if( a > b ){
     printf("The first number should be smaller than or equal to the second number\n");
     return 1;
   }

   // TODO: call the print_semiprimes function to print semiprimes in [a,b] (including a and b)
   print_semiprimes(a, b);
   printf("\n");
   return 0;
}


/*
 * TODO: implement this function to check the number is prime or not.
 * Input    : a number
 * Return   : 0 if the number is not prime, else 1
 */
int is_prime(int number)
{
  int i;
  for (i=2; i < number-1; i++)
  if (number%i == 0)      /* checks if there are any factors that evenly divide  */
  {                       /* into the number being checked for primality. */
    return 0;
  }
  return 1;
}

/*
 * TODO: implement this function to print all semiprimes in [a,b] (including a, b).
 * Input   : a, b (a should be smaller than or equal to b)
 * Return  : 0 if there is no semiprime in [a,b], else 1
 */
int print_semiprimes(int a, int b)
{
  int i,j,k = 0;
    for(i=a;i<=b;i++){
      for(j=2;j<=i-1;j++){
        if(i%j != 0){            /* this skips trying to check if a number is prime if i/j */
          continue;              /* is not and goes to next iteration of j */

        }
        if(is_prime(j) != 0 && is_prime(i/j) != 0){
          k += 1;                /* used to count the number of semiprimes in the range*/
          printf("%d ", i);      /* prints the number that is being checked if it is semiprime */
          break;                 /* once the number being checked is printed, break out of loop */
        }                        /* and move onto the next number in the range. */
      }
    }
  if (k != 0) return 1;
  else return 0;
}
