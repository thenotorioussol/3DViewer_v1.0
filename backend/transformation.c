#include "transformation.h"

void scale(matrix_t *coordinates, double scale_rate) {
  for (unsigned int i = 0; i < coordinates->rows; i++) {
    coordinates->matrix[i][x] *= scale_rate;
    coordinates->matrix[i][y] *= scale_rate;
    coordinates->matrix[i][z] *= scale_rate;
  }
}

void move_coords(obj_data_t *obj_data, double offset, int axis) {
  for (unsigned int i = 0; i < obj_data->count_of_vertexes; i++) {
    obj_data->coordinates.matrix[i][axis] += offset;
  }
}

void rotation_x(matrix_t *coordinates, double angle) {
  double temp_y = 0, temp_z = 0;
  for (unsigned int i = 0; i < coordinates->rows; i++) {
    temp_y = coordinates->matrix[i][y];
    temp_z = coordinates->matrix[i][z];
    coordinates->matrix[i][y] = temp_y * cos(angle) - temp_z * sin(angle);
    coordinates->matrix[i][z] = temp_y * sin(angle) + temp_z * cos(angle);
  }
}

void rotation_y(matrix_t *coordinates, double angle) {
  double temp_x = 0, temp_z = 0;
  for (unsigned int i = 0; i < coordinates->rows; i++) {
    temp_x = coordinates->matrix[i][x];
    temp_z = coordinates->matrix[i][z];
    coordinates->matrix[i][x] = temp_x * cos(angle) + temp_z * sin(angle);
    coordinates->matrix[i][z] = -temp_x * sin(angle) + temp_z * cos(angle);
  }
}

void rotation_z(matrix_t *coordinates, double angle) {
  double temp_x = 0, temp_y = 0;
  for (unsigned int i = 0; i < coordinates->rows; i++) {
    temp_x = coordinates->matrix[i][x];
    temp_y = coordinates->matrix[i][y];
    coordinates->matrix[i][x] = temp_x * cos(angle) - temp_y * sin(angle);

    coordinates->matrix[i][y] = temp_x * sin(angle) + temp_y * cos(angle);
  }
}

void centralization(obj_data_t *obj_data) {
  double min_values[3] = {obj_data->coordinates.matrix[0][x],
                          obj_data->coordinates.matrix[0][y],
                          obj_data->coordinates.matrix[0][z]};
  double max_values[3] = {obj_data->coordinates.matrix[0][x],
                          obj_data->coordinates.matrix[0][y],
                          obj_data->coordinates.matrix[0][z]};

  for (unsigned int i = 0; i < obj_data->count_of_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      if (min_values[j] > obj_data->coordinates.matrix[i][j]) {
        min_values[j] = obj_data->coordinates.matrix[i][j];
      }

      if (max_values[j] < obj_data->coordinates.matrix[i][j]) {
        max_values[j] = obj_data->coordinates.matrix[i][j];
      }
    }
  }

  double diffX = (max_values[x] - min_values[x]) / 2;
  double diffY = (max_values[y] - min_values[y]) / 2;
  double diffZ = (max_values[z] - min_values[z]) / 2;

  double min = min_values[0], max = max_values[0];
  for (int i = 0; i < 3; i++) {
    if (min > min_values[i]) min = min_values[i];
    if (max < max_values[i]) max = max_values[i];
  }
  obj_data->min_coord = min;
  obj_data->max_coord = max;

  move_coords(obj_data, -diffX, x);
  move_coords(obj_data, -diffY, y);
  move_coords(obj_data, -diffZ, z);
}

void projectionModel(obj_data_t *obj_data) {
  double temp = 0;
  for (unsigned int i = 0; i < obj_data->count_of_vertexes; i++) {
    temp = (obj_data->max_coord * 5) /
           (obj_data->max_coord * 5 + (-obj_data->coordinates.matrix[i][2] +
                                       obj_data->max_coord * obj_data->scale));
    obj_data->coordinates.matrix[i][0] =
        obj_data->coordinates.matrix[i][0] * temp;
    obj_data->coordinates.matrix[i][1] =
        obj_data->coordinates.matrix[i][1] * temp;
  }
}
