/**********************************************
- written by H. Lee 10/25/2022 
**********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MATRIX_METHODS.h"


double** constructHaarMatrixRecursive(int n);
double** concatenateTwoMatrices(double** hl, double** hr, int n);
double** applyKroneckerProduct(double** A, int n, double a, double b); 
/*
void printMatrix(double** A, int m, int n, char name[]); 
double** constructIdentity(int k); 
double** allocateMemory(int m, int n); 
void releaseMemory(double** A, int m);
*/


double** constructHaarMatrixRecursive(int n) {
	double** h;
	if (n > 2)
		h = constructHaarMatrixRecursive(n/2);
	else {
		//double** h;
		h = allocateMemory(2,2);
		h[0][0] = 1; h[0][1] = 1; h[1][0] = 1; h[1][1] = -1; //H = [1 1; 1 -1]
		return h; 
	}

	// construct left part (Kronecket product of h and [1;1]
	double** Hl = applyKroneckerProduct(h, n, 1, 1);
	releaseMemory(h, n/2);

	// construct right part (Kronecker product of I and [1;-1]
	double** I = constructIdentity(n/2);
	double** Hr = applyKroneckerProduct(I, n, 1, -1); 
	releaseMemory(I, n/2);


	// merge hl and hr
	double** H = concatenateTwoMatrices(Hl, Hr, n); //H = [hl hr]
	releaseMemory(Hl, n);
	releaseMemory(Hr, n);

	return H;
}

double** applyKroneckerProduct(double** A, int n, double a, double b) {
	double** h = allocateMemory(n, n/2);
	
	for (int j = 0; j < n/2; j++) {
		for (int i = 0; i < n/2; i++) {
			h[2*i][j] = A[i][j] * a;
			h[2*i+1][j] = A[i][j] * b;
		}
	}
	return h;
}

double** concatenateTwoMatrices(double** hl, double** hr, int n) {
	double** H = allocateMemory(n, n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (j < n/2)
				H[i][j] = hl[i][j];
			else
				H[i][j] = hr[i][j-n/2];
		}
	}
	return H; 
}

/*
void printMatrix(double** A, int m, int n, char name[]) {
	printf("\n%s = \n", name);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			printf("%lf ", A[i][j]);
		printf("\n");
	}
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

float** constructIdentity(int k) {
	float** I;
	I = (float**)malloc(sizeof(float*) * k);
	for (int i = 0; i < k; i++)
		I[i] = (float*)malloc(size(float*) * k);

	for (int i = 0; i < k; i++) {
		for (int j = 0; j < k; j++) {
			if (i != j)
				I[i][j] = 0.0;
			else
				I[i][j] = 1.0;
		}
	}

	return I;
}*/
	
