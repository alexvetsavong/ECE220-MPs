#include <stdio.h>
#include <math.h>

#define M_PI 3.14159265358979323846

/*
 * main
 *  DESCRIPTION: compute a set of points of a function and print the results
 *  INPUTS: console (int, float float) -- equation related constants
 *  OUTPUTS: console (x, f(x)) -- sequence of values to be printed
 *  RETURN VALUE: 0
 *  SIDE EFFECTS: none
 */
int main()
{
    // Declare variables
    int i, n;
    float w1, w2, x, result;
    // Prompt user for input
    printf("Please enter n, w1, w2: ");
    // Get user input
    scanf("%d %f %f", &n, &w1, &w2);
    // Compute function output
    /* for i from 0 to n-1
     *     compute and print xi and f(xi)
     *     use sin() function from math.h
     */
    for(i=0;i<n-1;i++){
      x = i*M_PI/n;
      result = sin(w1*x) + 0.5*sin(w2*x);
      printf("Result: (%f,%f)\n", x, result);
    }
    return 0;
}
