#ifndef SRC_TRANSFORMATION_H
#define SRC_TRANSFORMATION_H

#include <math.h>

#include "reader.h"

void move_coords(obj_data_t *obj_data, double offset, int axis);
void scale(matrix_t *coordinates, double scale_rate);

void rotation_x(matrix_t *coordinates, double angle);
void rotation_y(matrix_t *coordinates, double angle);
void rotation_z(matrix_t *coordinates, double angle);

void centralization(obj_data_t *obj_data);
void projectionModel(obj_data_t *obj_data);

enum axis {
  x = 0,
  y = 1,
  z = 2,
};

#endif  // SRC_TRANSFORMATION_H
