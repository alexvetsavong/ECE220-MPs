/* Function matrix_multiply
 * matrixA, matrixB, and matrix C are matrices represented as
 * one-dimensional arrays in row-major order. This function must
 * preform matrix multiplication so that C=A*B. 
 * INPUT: matrixA, a one dimensional array of size m*k
 *        matrixB, a one dimensional double array of size k*n
 * OUTPUT: matrixC, a one dimensional double array of size m*n
 */
void matrix_multiply(double *matrixA,double *matrixB,double *matrixC,int m,int k,int n)
{
	int x,y,z;
    	int a,b,c;

	for (x=0;x<m;x++){
		for(y=0;y<n;y++){
			for(z=0;z<k;z++){
				a = x * k + z;
		       		b = y + n * z; 
				c = x*n + y;
				matrixC[c] += matrixA[a] * matrixB[b];	
		}
	}
}
}

