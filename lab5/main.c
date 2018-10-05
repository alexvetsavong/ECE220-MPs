/* Code to simulate rolling three six-sided dice (D6)
 * User first types in seed value
 * Use seed value as argument to srand()
 * Call roll_three to generate three integers, 1-6
 * Print result "%d %d %d "
 * If triple, print "Triple!\n"
 * If it is not a triple but it is a dobule, print "Double!\n"
 * Otherwise print "\n"
 */

#include <stdio.h>
#include <stdlib.h>
#include "dice.h"

void srand(unsigned int seed){}

int main(){
	unsigned int seed, one, two, three;
	printf("Enter a seed: \n");
	scanf("%u", &seed);
	srand((unsigned) seed);
	roll_three(&one, &two, &three);
	printf("%u %u %u ", one, two, three);
	if (one == two == three){
		printf("Triple!\n");
	}
	else if (one == two || one == three || two == three) printf("Double!\n");
	return 0;
}	
