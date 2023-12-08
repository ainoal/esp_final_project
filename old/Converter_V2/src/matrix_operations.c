/*
 * matrix_operations.c
 *
 *  Created on: 02 Nov 2023
 *      Author: z103112
 */
// matrix_operations.c

#include "matrix_operations.h"

void matrix_vector_multiply(double mat[N][N], double vec[N], double result[N]) {
    for (int i = 0; i < N; ++i) {
        result[i] = 0;
        for (int j = 0; j < N; ++j) {
            result[i] += mat[i][j] * vec[j];
        }
    }
}

void add_vectors(double vec1[N], double vec2[N], double result[N]) {
    for (int i = 0; i < N; ++i) {
        result[i] = vec1[i] + vec2[i];
    }
}

double output(double mat[P][N], double vec[N]) {
    double result = 0;
    for (int i = 0; i < N; ++i) {
        result += mat[0][i] * vec[i];
    }
    return result;
}


