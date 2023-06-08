#include "reader.h"

int read_file(char *file_path, obj_data_t *obj_data) {
  int flag = read_file1(file_path, obj_data);
  if (flag == OK) {
    flag = read_file2(file_path, obj_data);
  }
  return flag;
}

int read_file1(char *filename, obj_data_t *obj_data) {
  FILE *file = fopen(filename, "r");
  int flag = file_checker(file);
  if (flag != OK) {
    return flag;
  }

  unsigned int count_of_vertexes = 0, count_of_facets = 0;
  int ch1 = 0, ch2 = 0;

  while (ch2 != EOF) {
    ch1 = fgetc(file);
    if (ch1 != EOF) {
      ch2 = fgetc(file);
      if (ch1 == 'v' && ch2 == ' ') {
        count_of_vertexes++;
      } else if (ch1 == 'f' && ch2 == ' ') {
        count_of_facets++;
      }
      skip_line(file);
    } else {
      ch2 = EOF;
    }
  }
  if (count_of_vertexes < 3 || count_of_facets < 1) {
    flag = INCORRECT_DATA;
  }
  obj_data->count_of_vertexes = count_of_vertexes;
  obj_data->count_of_facets = count_of_facets;

  fclose(file);
  return flag;
}

int read_file2(char *filename, obj_data_t *obj_data) {
  FILE *file = fopen(filename, "r");
  int flag = file_checker(file);

  if (flag != OK) {
    return flag;
  }

  s21_create_matrix(obj_data->count_of_vertexes + 1, 3, &obj_data->coordinates);
  char buffer[256] = {0};
  obj_data->polygons = calloc(obj_data->count_of_facets + 1, sizeof(polygon_t));

  int ch1 = 0, ch2 = 0;
  unsigned int row = 0, poly_index = 0;

  while (ch2 != EOF && flag == OK) {
    ch1 = fgetc(file);
    if (ch1 != EOF) {
      ch2 = fgetc(file);
      if (ch1 == 'v' && ch2 == ' ') {
        flag = parse_vertex(file, obj_data, row);
        row++;
      } else if (ch1 == 'f' && ch2 == ' ') {
        flag = parse_facets(file, obj_data, poly_index, buffer);
        poly_index++;
      }

      if (ch1 != 'f') {
        skip_line(file);
      }

    } else {
      ch2 = EOF;
    }
  }

  fclose(file);
  return flag;
}

int file_checker(FILE *file) {
  int flag = OK;
  if (file == NULL) {
    flag = FILE_ERROR;
  }
  if (flag == OK) {
    if (fgetc(file) == EOF) {
      fclose(file);
      flag = FILE_IS_EMPTY;
    } else {
      fseek(file, 0, 0);
    }
  }
  return flag;
}

void set_started(obj_data_t *obj_data) {
  obj_data->offset.x = 0;
  obj_data->offset.y = 0;
  obj_data->offset.z = 0;
  obj_data->scale = 1;
  obj_data->angle.x = 0;
  obj_data->angle.y = 0;
  obj_data->angle.z = 0;
}

void determination_max_min(obj_data_t *obj_data, int x, int y) {
  if (!x && !y) {
    obj_data->min_coord = obj_data->coordinates.matrix[x][y];
    obj_data->max_coord = obj_data->coordinates.matrix[x][y];
  }
  if (obj_data->min_coord > obj_data->coordinates.matrix[x][y])
    obj_data->min_coord = obj_data->coordinates.matrix[x][y];
  if (obj_data->max_coord < obj_data->coordinates.matrix[x][y])
    obj_data->max_coord = obj_data->coordinates.matrix[x][y];
}

int is_digit(char ch) { return ch >= '0' && ch <= '9'; }

void data_cleaner(obj_data_t *obj_data) {
  for (unsigned int i = 0; i < obj_data->count_of_facets; i++) {
    free(obj_data->polygons[i].vertexes);
  }
  obj_data->count_of_vertexes = 0;
  obj_data->count_of_facets = 0;
  free(obj_data->polygons);
  s21_remove_matrix(&obj_data->coordinates);
}

int parse_facets(FILE *file, obj_data_t *obj_data, unsigned int poly_index,
                 char *buffer) {
  int flag = OK;

  if (poly_index > obj_data->count_of_facets + 1) {
    flag = INCORRECT_DATA;
  }

  if (fgets(buffer, 256, file) == NULL) {
    flag = INCORRECT_DATA;
  } else {
    int vertex_count = get_vertexes_count(buffer);
    if (vertex_count < 3) {
      flag = INCORRECT_DATA;
    }
    obj_data->polygons[poly_index].numbers_of_vertexes = vertex_count;
    obj_data->polygons[poly_index].vertexes = calloc(vertex_count, sizeof(int));
    char number_ch[16] = {0};

    for (int i = 0, j = 0, v = 0, space_flag = 1;
         buffer[i] != '\0' && v < vertex_count; i++) {
      if (is_digit(buffer[i]) && space_flag) {
        number_ch[j] = buffer[i];
        j++;
      } else if (buffer[i] == '/') {
        space_flag = 0;
      } else if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\r') {
        space_flag = 1;
        obj_data->polygons[poly_index].vertexes[v] = atoi(number_ch) - 1;
        memset(number_ch, 0, 16);
        v++;
        j = 0;
      }
    }
  }

  return flag;
}

int parse_vertex(FILE *file, obj_data_t *obj_data, unsigned int i) {
  int flag = OK;

  if (i > obj_data->count_of_vertexes + 1) {
    flag = INCORRECT_DATA;
  }

  if (flag == OK) {
    double x = 0, y = 0, z = 0;
    fscanf(file, "%lf %lf %lf", &x, &y, &z);
    obj_data->coordinates.matrix[i][0] = x;
    obj_data->coordinates.matrix[i][1] = y;
    obj_data->coordinates.matrix[i][2] = z;
  }

  return flag;
}

void skip_line(FILE *file) {
  int ch = fgetc(file);
  while (ch != EOF && ch != '\n') {
    ch = fgetc(file);
  }
}

int get_vertexes_count(char *str) {
  int vertexes_count = 0, flag_space = 1;

  for (int i = 0; str[i] != '\0'; i++) {
    if (is_digit(str[i]) && flag_space) {
      vertexes_count++;
      flag_space = 0;
    } else if (str[i] == ' ') {
      flag_space = 1;
    }
  }

  return vertexes_count;
}
