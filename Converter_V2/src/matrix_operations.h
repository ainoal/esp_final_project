// matrix_operations.h

#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

// System dimensions
#define N 6 // Dimension of state vector x
#define M 1 // Dimension of input vector u
#define P 1 // Dimension of output vector y

// Function prototypes
void matrix_vector_multiply(double mat[N][N], double vec[N], double result[N]);
void add_vectors(double vec1[N], double vec2[N], double result[N]);
double output(double mat[P][N], double vec[N]);

#endif // MATRIX_OPERATIONS_H
