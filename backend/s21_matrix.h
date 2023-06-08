#ifndef SRC_S21_MATRIX_H
#define SRC_S21_MATRIX_H

#include <stdlib.h>

#define S21_SUCCESS 0
#define S21_INCORRECT 1

typedef struct matix_struct {
  double **matrix;
  unsigned int rows;
  int columns;
} matrix_t;

int s21_create_matrix(unsigned int rows, int columns, matrix_t *result);
void s21_copy_matrix(matrix_t *dist, matrix_t *source);
void s21_remove_matrix(matrix_t *A);
int is_correct(matrix_t *m1);

#endif  // SRC_S21_MATRIX_H
