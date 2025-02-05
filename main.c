#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "matmult_funs.h"

// Matrix allocation and utility functions
double **allocate_matrix(int rows, int cols);
void free_matrix(double **matrix);
void fill_matrix(double **matrix, int rows, int cols, double start);

// Performance measurement function
void measure_performance(void (*matmult_func)(int, int, int, double **, double **, double **), 
                         int m, int n, int k, double **A, double **B, double **C);

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <m> <n> <k> <version>\n", argv[0]);
        return 1;
    }

    // Parse command-line arguments
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    int k = atoi(argv[3]);
    char *version = argv[4];

    // Allocate and fill matrices
    double **A = allocate_matrix(m, k);
    double **B = allocate_matrix(k, n);
    double **C = allocate_matrix(m, n);

    fill_matrix(A, m, k, 10);
    fill_matrix(B, k, n, 20);

    // Select the appropriate matrix multiplication function
    if (strcmp(version, "mnk") == 0) {
        measure_performance(matmult_mnk, m, n, k, A, B, C);
    } else if (strcmp(version, "mkn") == 0) {
        measure_performance(matmult_mkn, m, n, k, A, B, C);
    } else if (strcmp(version, "nmk") == 0) {
        measure_performance(matmult_nmk, m, n, k, A, B, C);
    } else if (strcmp(version, "nkm") == 0) {
        measure_performance(matmult_nkm, m, n, k, A, B, C);
    } else if (strcmp(version, "kmn") == 0) {
        measure_performance(matmult_kmn, m, n, k, A, B, C);
    } else if (strcmp(version, "knm") == 0) {
        measure_performance(matmult_knm, m, n, k, A, B, C);
    }
    // Add additional else-if blocks here for other versions
    else {
        printf("Unknown version: %s\n", version);
    }

    // Free matrices
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);

    return 0;
}

// Utility functions
// allocate a double-prec m x n matrix
double** allocate_matrix(int m, int n) {
    if (m <= 0 || n <= 0) return NULL;
    double** A = malloc(m * sizeof(double*));
    if (A == NULL) return NULL;
    A[0] = malloc(m*n*sizeof(double));
    if (A[0] == NULL) {
        free(A);
        return NULL;
    }
    for (int i = 1; i < m; i++)
        A[i] = A[0] + i * n;
    return A;
}

// de-allocting memory, allocated with
// dmalloc_2d
void free_matrix(double** A) {
    free(A[0]);
    free(A);
}

void fill_matrix(double** mtx, int m, int n, double start) {
    for (int i = 0; i < m; i++){
        for (int j = 0; j < n; j++){
            mtx[i][j] = start + (double) (i*m + j);
        }
    }
}

void measure_performance(void (*matmult_func)(int, int, int, double **, double **, double **), 
                         int m, int n, int k, double **A, double **B, double **C) {
    clock_t start = clock();
    clock_t end;
    int iterations = 0;

    do {
        matmult_func(m, n, k, A, B, C);
        iterations++;
        end = clock();
    } while ((double)(end - start) / CLOCKS_PER_SEC < 3.0);

    double duration = (double)(end - start) / CLOCKS_PER_SEC;
    long flops_per_mult = 2L * m * n * k;  // 2 FLOPs per element: one multiply and one add
    long total_flops_per_sec = iterations * flops_per_mult / duration;
    long mem_footprint = m * k * 8 + k * n * 8 + m * n * 8;

    printf("%ld\t%ld\n", mem_footprint, total_flops_per_sec);
}
