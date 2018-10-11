//Function for generating three d6 rolls
#include <stdlib.h>

void roll_three(int* one, int* two, int* three){
	int temp;
	temp = rand() % 7;
	*one = temp;
	
	temp = rand() % 7;
	*two = temp;
	
	temp = rand() % 7;
	*three = temp;
}
