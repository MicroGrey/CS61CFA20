#include "matrix.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

/* Below are some intel intrinsics that might be useful
 * void _mm256_storeu_pd (double * mem_addr, __m256d a)
 * __m256d _mm256_set1_pd (double a)
 * __m256d _mm256_set_pd (double e3, double e2, double e1, double e0)
 * __m256d _mm256_loadu_pd (double const * mem_addr)
 * __m256d _mm256_add_pd (__m256d a, __m256d b)
 * __m256d _mm256_sub_pd (__m256d a, __m256d b)
 * __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
 * __m256d _mm256_mul_pd (__m256d a, __m256d b)
 * __m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
 * __m256d _mm256_and_pd (__m256d a, __m256d b)
 * __m256d _mm256_max_pd (__m256d a, __m256d b)
*/

/*
 * Generates a random double between `low` and `high`.
 */
double rand_double(double low, double high) {
    double range = (high - low);
    double div = RAND_MAX / range;
    return low + (rand() / div);
}

/*
 * Generates a random matrix with `seed`.
 */
void rand_matrix(matrix *result, unsigned int seed, double low, double high) {
    srand(seed);
    for (int i = 0; i < result->rows; i++) {
        for (int j = 0; j < result->cols; j++) {
            set(result, i, j, rand_double(low, high));
        }
    }
}

/*
 * Allocate space for a matrix struct pointed to by the double pointer mat with
 * `rows` rows and `cols` columns. You should also allocate memory for the data array
 * and initialize all entries to be zeros. Remember to set all fieds of the matrix struct.
 * `parent` should be set to NULL to indicate that this matrix is not a slice.
 * You should return -1 if either `rows` or `cols` or both have invalid values, or if any
 * call to allocate memory in this function fails. If you don't set python error messages here upon
 * failure, then remember to set it in numc.c.
 * Return 0 upon success and non-zero upon failure.
 */
int allocate_matrix(matrix **mat, int rows, int cols) {
    /* TODO: YOUR CODE HERE */
    if (cols <= 0 || rows <= 0) {
        PyErr_SetString(PyExc_ValueError, "rows or cols invalid");
        return -1;
    }
    *mat = (matrix *)malloc(sizeof(matrix));
    if (*mat == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Allocate matrix failed");
        return -1;
    }
    (*mat)->rows = rows;
    (*mat)->cols = cols;
    (*mat)->is_1d = rows == 1 || cols == 1;
    (*mat)->ref_cnt = 1;
    (*mat)->parent = NULL;
    (*mat)->data = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        (*mat)->data[i] = (double *)calloc(cols, sizeof(double));
        if ((*mat)->data[i] == NULL) {
            PyErr_SetString(PyExc_RuntimeError, "Allocate matrix failed");
            return -1;
        }
    }
    return 0;
}

/*
 * Allocate space for a matrix struct pointed to by `mat` with `rows` rows and `cols` columns.
 * This is equivalent to setting the new matrix to be
 * from[row_offset:row_offset + rows, col_offset:col_offset + cols]
 * If you don't set python error messages here upon failure, then remember to set it in numc.c.
 * Return 0 upon success and non-zero upon failure.
 */
int allocate_matrix_ref(matrix **mat, matrix *from, int row_offset, int col_offset,
                        int rows, int cols) {
    /* TODO: YOUR CODE HERE */
    *mat = (matrix *)malloc(sizeof(matrix));
    if (*mat == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Allocate matrix failed");
        return -1;
    }
    if (cols <= 0 || rows <= 0 || row_offset < 0 || col_offset < 0 || row_offset + rows > from->rows || col_offset + cols > from->cols) {
        PyErr_SetString(PyExc_ValueError, "Invalid arguments");
        return -1;
    }
    (*mat)->rows = rows;
    (*mat)->cols = cols;
    (*mat)->is_1d = rows == 1 || cols == 1;
    (*mat)->ref_cnt = 1;
    from->ref_cnt++;
    (*mat)->parent = from;
    (*mat)->data = from->data + row_offset;
    for (int i = 0; i < rows; i++) {
        (*mat)->data[i] = (double *)calloc(cols, sizeof(double));
        if ((*mat)->data[i] == NULL) {
            PyErr_SetString(PyExc_RuntimeError, "Allocate matrix failed");
            return -1;
        }
        for (int j = 0; j < cols; j++) {
            (*mat)->data[i] = from->data[i] + col_offset;
        }
    }
    return 0;
}

/*
 * This function will be called automatically by Python when a numc matrix loses all of its
 * reference pointers.
 * You need to make sure that you only free `mat->data` if no other existing matrices are also
 * referring this data array.
 * See the spec for more information.
 */
void deallocate_matrix(matrix *mat) {
    /* TODO: YOUR CODE HERE */
    if (mat == NULL) {
        return;
    }
    if (mat->ref_cnt == 1) {
        for (int i = 0; i < mat->rows; i++) {
            free(mat->data[i]);
        }
        free(mat->data);
        free(mat);
    }
}

/*
 * Return the double value of the matrix at the given row and column.
 * You may assume `row` and `col` are valid.
 */
double get(matrix *mat, int row, int col) {
    /* TODO: YOUR CODE HERE */
    return mat->data[row][col];
}

/*
 * Set the value at the given row and column to val. You may assume `row` and
 * `col` are valid
 */
void set(matrix *mat, int row, int col, double val) {
    /* TODO: YOUR CODE HERE */
    mat->data[row][col] = val;
}

/*
 * Set all entries in mat to val
 */
void fill_matrix(matrix *mat, double val) {
    /* TODO: YOUR CODE HERE */
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            set(mat, i, j, val);
        }
    }
}

/*
 * Store the result of adding mat1 and mat2 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int add_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    /* TODO: YOUR CODE HERE */
    if (mat1->rows != mat2->rows || mat1->cols != mat2->cols) {
        PyErr_SetString(PyExc_ValueError, "mat1 and mat2 have different shapes");
        return -1;
    }
    if (result->rows != mat1->rows || result->cols != mat1->cols) {
        PyErr_SetString(PyExc_ValueError, "result has different shape");
        return -1;
    }
    int rows = mat1->rows;
    int cols = mat1->cols;
    #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            set(result, i, j, get(mat1, i, j) + get(mat2, i, j));
        }
    }
    return 0;
}

/*
 * Store the result of subtracting mat2 from mat1 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int sub_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    /* TODO: YOUR CODE HERE */
    if (mat1->rows != mat2->rows || mat1->cols != mat2->cols) {
        PyErr_SetString(PyExc_ValueError, "mat1 and mat2 have different shapes");
        return -1;
    }
    if (result->rows != mat1->rows || result->cols != mat1->cols) {
        PyErr_SetString(PyExc_ValueError, "result has different shape");
        return -1;
    }
    int rows = mat1->rows;
    int cols = mat1->cols;
    #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            set(result, i, j, get(mat1, i, j) - get(mat2, i, j));
        }
    }
    return 0;
}

/*
 * Store the result of multiplying mat1 and mat2 to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 * Remember that matrix multiplication is not the same as multiplying individual elements.
 */
int mul_matrix(matrix *result, matrix *mat1, matrix *mat2) {
    /* TODO: YOUR CODE HERE */
    if (mat1 == NULL || mat2 == NULL || result == NULL) {
        PyErr_SetString(PyExc_ValueError, "mat1, mat2 or result is NULL");
        return -1;
    }
    if (mat1->cols != mat2->rows) {
        PyErr_SetString(PyExc_ValueError, "mat1 and mat2 have incompatible shapes");
        return -1;
    }
    if (result->rows != mat1->rows || result->cols != mat2->cols) {
        PyErr_SetString(PyExc_ValueError, "result has different shape");
        return -1;
    }
    matrix *temp_result = NULL;
    allocate_matrix(&temp_result, result->rows, result->cols);
    int rows = mat1->rows;
    int cols = mat1->cols;
    #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < cols; k++) {
                set(temp_result, i, j, get(temp_result, i, j) + get(mat1, i, k) * get(mat2, k, j));
            }
        }
    }
    result->data = temp_result->data;
    free(temp_result);
    return 0;
}

/*
 * Store the result of raising mat to the (pow)th power to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 * Remember that pow is defined with matrix multiplication, not element-wise multiplication.
 */
int pow_matrix(matrix *result, matrix *mat, int pow) {
    /* TODO: YOUR CODE HERE */
    if (mat->rows != mat->cols || pow < 0) {
        PyErr_SetString(PyExc_ValueError, "mat is not a square matrix or pow is negative");
        return -2;
    }
    if (mat->rows != result->rows || mat->cols != result->cols) {
        PyErr_SetString(PyExc_ValueError, " mat and result have different shapes");
        return -3;
    }

    fill_matrix(result, 0);
    for (int i = 0; i < mat->rows; i++) {
        set(result, i, i, 1);
    }
    for (int i = 0; i < pow; i++) {
        int errorcode = mul_matrix(result, result, mat);
        if (errorcode != 0) {
            return errorcode;
        }
    }
    return 0;
}

/*
 * Store the result of element-wise negating mat's entries to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
int neg_matrix(matrix *result, matrix *mat) {
    /* TODO: YOUR CODE HERE */
    if (result->rows != mat->rows || result->cols != mat->cols) {
        PyErr_SetString(PyExc_ValueError, "result has different shape");
        return -1;
    }
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            set(result, i, j, -get(mat, i, j));
        }
    }
    return 0;
}

/*
 * Store the result of taking the absolute value element-wise to `result`.
 * Return 0 upon success and a nonzero value upon failure.
 */
 double absolute(double num) {
    if (num < 0) {
        return -num;
    }
    return num;
 }
int abs_matrix(matrix *result, matrix *mat) {
    /* TODO: YOUR CODE HERE */
    if (result->rows != mat->rows || result->cols != mat->cols) {
        PyErr_SetString(PyExc_ValueError, "result has different shape");
        return -1;
    }
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            set(result, i, j, absolute(get(mat, i, j)));
        }
    }
    return 0;
}

