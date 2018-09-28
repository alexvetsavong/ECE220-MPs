#include <stdio.h>
#include "prime.h"

int main(){
int i, n;
  printf("Enter the value for n: ");
  scanf("%d", &n);
  printf("Printing primes less than or equal to %d\n", n);
  // Write the code to take a number n from user and print all the prime numbers between 1 and n
  for (i=1; i <= n; i++){
    if (is_prime(i) != 0){
      printf("%d ", i);
    }
  }
  printf("\n");
  return 0;
}
