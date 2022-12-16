#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MATRIX_METHODS.h"


//functions for convenience
double** allocateMemory(int m, int n) {
	double** A;
	A = (double**) malloc(sizeof(double*) * m);
	for (int i = 0; i < m; i++) {
		A[i] = (double*) malloc(sizeof(double) * n);
	}
	return A;
}


void releaseMemory(double** A, int m) {
	for (int i = 0; i < m; i++)
		free(A[i]);
	free(A);
}

void printMatrix(double** A, int m, int n, char name[]) {
	printf("\n%s = \n", name);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			printf("%5.2lf ", A[i][j]);
		printf("\n");
	}
}

//functions to implement in prj0 
double** transposeMatrix(double **A, int m, int n) {
	double** B = allocateMemory(n, m);

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			B[j][i] = A[i][j];	
	
	return B;
}	

double** normalizeVector(double** v, int m) {
	double** w;
	double len = 0.0;

	for (int i = 0; i < m; i++)
		len += v[i][0]*v[i][0];	
	len = sqrt(len);

	w = allocateMemory(m,1);
	for (int i = 0; i < m; i++)
		w[i][0] = v[i][0]/len;
	
	return w;
}

double** constructIdentity(int k) {
 	double** I = allocateMemory(k,k);
 
 	for (int i = 0; i < k; i++) {
 		for (int j = 0; j < k; j++) {
 			if (i != j) 
 				I[i][j] = 0.0;
 			else
 				I[i][j] = 1.0;
 		}
 	}
 	return I;
 }
