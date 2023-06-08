#ifndef SRC_READER_H
#define SRC_READER_H

#include <stdio.h>
#include <string.h>

#include "s21_matrix.h"

#define BUFFER 1048576

enum flags {
  OK = 1,
  FILE_ERROR = -1,
  FILE_IS_EMPTY = -2,
  INCORRECT_DATA = -3,
  INCORRECT_FILENAME = -4,
};

typedef struct facet {
  int *vertexes;
  int numbers_of_vertexes;
} polygon_t;

typedef struct object {
  double x, y, z;
} object_offset;

typedef struct angle {
  double x, y, z;
} object_angle;

typedef struct object_data {
  unsigned int count_of_vertexes;
  unsigned int count_of_facets;
  int max_coord;
  int min_coord;
  matrix_t coordinates;
  polygon_t *polygons;
  object_offset offset;
  object_angle angle;
  double scale;
} obj_data_t;

int read_file(char *file_path, obj_data_t *obj_data);
int file_checker(FILE *file);
int read_file1(char *filename, obj_data_t *obj_data);
int read_file2(char *filename, obj_data_t *obj_data);
int parse_facets(FILE *file, obj_data_t *obj_data, unsigned int poly_index,
                 char *buffer);
int parse_vertex(FILE *file, obj_data_t *obj_data, unsigned int i);
int get_vertexes_count(char *str);

void skip_line(FILE *file);
void determination_max_min(obj_data_t *obj_data, int x, int y);
int is_digit(char ch);
void data_cleaner(obj_data_t *obj_data);
void set_started(obj_data_t *obj_data);

#endif  // SRC_READER_H
