#include "s21_matrix.h"

int is_correct(matrix_t *m1) {
  int flag = S21_SUCCESS;
  if (m1 == NULL || m1->rows <= 0 || m1->columns <= 0) {
    flag = S21_INCORRECT;
  }
  return flag;
}

int s21_create_matrix(unsigned int rows, int columns, matrix_t *result) {
  int flag = S21_SUCCESS;
  if (rows < 1 || columns < 1) {
    result->rows = 0;
    result->columns = 0;
    flag = S21_INCORRECT;
  }
  if (flag == S21_SUCCESS) {
    result->matrix = (double **)calloc(rows, sizeof(double *));
    if (result->matrix == NULL) {
      flag = S21_INCORRECT;
    }
    if (flag == S21_SUCCESS) {
      result->rows = rows;
      result->columns = columns;
      for (unsigned int i = 0; i < rows; i++) {
        result->matrix[i] = (double *)calloc(columns, sizeof(double));
        if (result->matrix[i] == NULL) {
          flag = S21_INCORRECT;
          for (unsigned int j = 0; j < i; j++) {
            free(result->matrix[j]);
          }
          free(result->matrix);
          i = rows;
        }
      }
    }
  }
  return flag;
}

void s21_remove_matrix(matrix_t *A) {
  if (is_correct(A) == S21_SUCCESS) {
    for (unsigned i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
    A->matrix = NULL;
    A->rows = 0;
    A->columns = 0;
  }
}

void s21_copy_matrix(matrix_t *dist, matrix_t *source) {
  for (unsigned i = 0; i < source->rows; i++) {
    for (int j = 0; j < 3; j++) {
      dist->matrix[i][j] = source->matrix[i][j];
    }
  }
}
