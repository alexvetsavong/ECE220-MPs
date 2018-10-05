//Function for generating three d6 rolls
#include <stdlib.h>

int rand(){}

void roll_three(int* one, int* two, int* three){
	*one = rand() % 6;
	*two = rand() % 6;
	*three = rand() % 6;
}
