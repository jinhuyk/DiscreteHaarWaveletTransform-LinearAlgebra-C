//functions for convenience
double** allocateMemory(int m, int n); 
void releaseMemory(double** A, int m);
void printMatrix(double** A, int m, int n, char name[]);
double** constructIdentity(int k);

//functions to implement in prj0 
double** transposeMatrix(double** A, int m, int n);
double** normalizeVector(double** v, int n);
