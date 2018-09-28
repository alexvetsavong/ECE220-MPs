#include "prime.h"

int is_prime(int n){
  // Return 1 if n is a prime, and 0 if it is not
  int i;
  for (i=2; i < n-1; i++)
  if (n%i == 0)
  {
    return 0;
  }
  return 1;
}
