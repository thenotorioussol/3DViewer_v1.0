#include <check.h>

#include "backend/reader.h"
#include "backend/transformation.h"

START_TEST(test_reader1) {
  int flag = OK;
  {
    obj_data_t data = {0};
    flag = read_file("obj_files/chair.obj", &data);
    ck_assert_int_eq(data.count_of_vertexes, 40);
    ck_assert_int_eq(data.count_of_facets, 76);
    ck_assert_int_eq(flag, OK);
    data_cleaner(&data);
  }

  {
    obj_data_t data = {0};
    flag = read_file("obj_files/cottage.obj", &data);
    ck_assert_int_eq(data.count_of_vertexes, 353);
    ck_assert_int_eq(data.count_of_facets, 259);
    ck_assert_int_eq(flag, OK);
    data_cleaner(&data);
  }

  {
    obj_data_t data = {0};
    flag = read_file("obj_files/man.obj", &data);
    ck_assert_int_eq(data.count_of_vertexes, 24461);
    ck_assert_int_eq(data.count_of_facets, 24459);
    ck_assert_int_eq(flag, OK);
    data_cleaner(&data);
  }

  {
    obj_data_t data = {0};
    flag = read_file("obj_files/mount.obj", &data);
    ck_assert_int_eq(data.count_of_vertexes, 3604);
    ck_assert_int_eq(data.count_of_facets, 3482);
    ck_assert_int_eq(flag, OK);
    data_cleaner(&data);
  }

  {
    obj_data_t data = {0};
    flag = read_file("obj_files/tree.obj", &data);
    ck_assert_int_eq(data.count_of_vertexes, 281);
    ck_assert_int_eq(data.count_of_facets, 312);
    ck_assert_int_eq(flag, OK);
    data_cleaner(&data);
  }
}
END_TEST

START_TEST(test_reader2) {
  int flag = OK;

  obj_data_t data = {0};
  flag = read_file("obj_files/chair.obj", &data);
  ck_assert_int_eq(flag, OK);

  ck_assert_double_eq_tol(data.coordinates.matrix[1][0], -0.8, 1e-7);
  ck_assert_double_eq_tol(data.coordinates.matrix[1][1], 0, 1e-7);
  ck_assert_double_eq_tol(data.coordinates.matrix[1][2], 0.7, 1e-7);

  ck_assert_double_eq_tol(data.coordinates.matrix[31][0], 5.96046e-08, 1e-7);
  ck_assert_double_eq_tol(data.coordinates.matrix[31][1], 0, 1e-7);
  ck_assert_double_eq_tol(data.coordinates.matrix[31][2], 0.7, 1e-7);

  ck_assert_uint_eq(data.polygons[1].vertexes[0], 3);
  ck_assert_uint_eq(data.polygons[1].vertexes[1], 1);
  ck_assert_uint_eq(data.polygons[1].vertexes[2], 0);

  data_cleaner(&data);
}
END_TEST

START_TEST(test_error_file) {
  int flag = OK;
  {
    obj_data_t data = {0};
    flag = read_file("obj_files/softsunami.obj", &data);
    ck_assert_int_eq(data.count_of_vertexes, 0);
    ck_assert_int_eq(data.count_of_facets, 0);
    ck_assert_int_eq(flag, FILE_ERROR);
  }
}
END_TEST

START_TEST(test_scale) {
  int flag = OK;

  obj_data_t data = {0};
  flag = read_file1("obj_files/chair.obj", &data);
  flag = read_file2("obj_files/chair.obj", &data);
  ck_assert_int_eq(flag, OK);

  scale(&data.coordinates, 2);

  ck_assert_double_eq_tol(data.coordinates.matrix[1][0], -0.8 * 2, 1e-7);
  ck_assert_double_eq_tol(data.coordinates.matrix[1][1], 0, 1e-7);
  ck_assert_double_eq_tol(data.coordinates.matrix[1][2], 0.7 * 2, 1e-7);

  ck_assert_double_eq_tol(data.coordinates.matrix[31][0], 5.96046e-08 * 2,
                          1e-7);
  ck_assert_double_eq_tol(data.coordinates.matrix[31][1], 0, 1e-7);
  ck_assert_double_eq_tol(data.coordinates.matrix[31][2], 0.7 * 2, 1e-7);

  scale(&data.coordinates, 0);

  ck_assert_double_eq_tol(data.coordinates.matrix[1][0], 0, 1e-7);
  ck_assert_double_eq_tol(data.coordinates.matrix[1][1], 0, 1e-7);
  ck_assert_double_eq_tol(data.coordinates.matrix[1][2], 0, 1e-7);

  data_cleaner(&data);
}
END_TEST

START_TEST(test_move) {
  int flag = OK;

  obj_data_t data = {0};
  flag = read_file1("obj_files/chair.obj", &data);
  flag = read_file2("obj_files/chair.obj", &data);
  ck_assert_int_eq(flag, OK);

  move_coords(&data, 15, x);

  ck_assert_double_eq_tol(data.coordinates.matrix[1][0], -0.8 + 15, 1e-7);
  ck_assert_double_eq_tol(data.coordinates.matrix[1][1], 0, 1e-7);
  ck_assert_double_eq_tol(data.coordinates.matrix[1][2], 0.7, 1e-7);

  move_coords(&data, -5, y);

  ck_assert_double_eq_tol(data.coordinates.matrix[31][0], 5.96046e-08 + 15,
                          1e-7);
  ck_assert_double_eq_tol(data.coordinates.matrix[31][1], 0 - 5, 1e-7);
  ck_assert_double_eq_tol(data.coordinates.matrix[31][2], 0.7, 1e-7);

  move_coords(&data, 0, z);

  ck_assert_double_eq_tol(data.coordinates.matrix[31][0], 5.96046e-08 + 15,
                          1e-7);
  ck_assert_double_eq_tol(data.coordinates.matrix[31][1], 0 - 5, 1e-7);
  ck_assert_double_eq_tol(data.coordinates.matrix[31][2], 0.7, 1e-7);

  data_cleaner(&data);
}
END_TEST

START_TEST(test_rotation) {
  int flag = OK;

  obj_data_t data = {0};
  flag = read_file1("obj_files/tree.obj", &data);
  flag = read_file2("obj_files/tree.obj", &data);
  ck_assert_int_eq(flag, OK);

  rotation_x(&data.coordinates, 10);
  rotation_y(&data.coordinates, 10);
  rotation_z(&data.coordinates, 10);
  data_cleaner(&data);
}
END_TEST

START_TEST(test_matrix) {
  matrix_t m1;
  int result;

  result = s21_create_matrix(1, 3, &m1);
  ck_assert_int_eq(result, S21_SUCCESS);
  s21_remove_matrix(&m1);

  result = s21_create_matrix(3, 1, &m1);
  ck_assert_int_eq(result, S21_SUCCESS);
  s21_remove_matrix(&m1);

  result = s21_create_matrix(0, 2, &m1);
  ck_assert_int_eq(result, S21_INCORRECT);
  s21_remove_matrix(&m1);

  result = s21_create_matrix(-1, -5, &m1);
  ck_assert_int_eq(result, S21_INCORRECT);
  s21_remove_matrix(&m1);
}
END_TEST

START_TEST(test_set_started) {
  obj_data_t data = {0};
  read_file1("obj_files/tree.obj", &data);
  data.offset.x = 12;
  data.offset.y = 13;
  data.offset.z = 4;
  data.scale = 5;
  data.angle.x = 2;
  data.angle.y = 2;
  data.angle.z = 20;
  set_started(&data);
  ck_assert_double_eq_tol(data.offset.x, 0.0, 1e-7);
  ck_assert_double_eq_tol(data.offset.y, 0.0, 1e-7);
  ck_assert_double_eq_tol(data.offset.z, 0.0, 1e-7);
  ck_assert_double_eq_tol(data.scale, 1.0, 1e-7);
  ck_assert_double_eq_tol(data.angle.x, 0.0, 1e-7);
  ck_assert_double_eq_tol(data.angle.y, 0.0, 1e-7);
  ck_assert_double_eq_tol(data.angle.z, 0.0, 1e-7);
}
END_TEST

START_TEST(test_max_min) {
  obj_data_t data = {0};
  s21_create_matrix(3, 3, &data.coordinates);
  for (unsigned int i = 0; i < 3; ++i) {
    for (unsigned int j = 0; j < 3; ++j) {
      data.coordinates.matrix[i][j] = i + j;
    }
  }
  data.coordinates.matrix[2][1] = -2;
  for (unsigned int i = 0; i < 3; ++i) {
    for (unsigned int j = 0; j < 3; ++j) {
      determination_max_min(&data, i, j);
    }
  }
  ck_assert_double_eq_tol(data.max_coord, 4.0, 1e-7);
  ck_assert_double_eq_tol(data.min_coord, -2.0, 1e-7);
  s21_remove_matrix(&data.coordinates);
}
END_TEST

START_TEST(test_copy_matrix) {
  matrix_t dataOne = {0};
  matrix_t dataTwo = {0};
  s21_create_matrix(3, 3, &dataOne);
  s21_create_matrix(3, 3, &dataTwo);
  for (unsigned int i = 0; i < 3; ++i) {
    for (unsigned int j = 0; j < 3; ++j) {
      dataOne.matrix[i][j] = i * j;
    }
  }
  s21_copy_matrix(&dataTwo, &dataOne);
  for (unsigned int i = 0; i < 3; ++i) {
    for (unsigned int j = 0; j < 3; ++j) {
      ck_assert_double_eq_tol(dataOne.matrix[i][j], dataTwo.matrix[i][j], 1e-7);
    }
  }
  s21_remove_matrix(&dataOne);
  s21_remove_matrix(&dataTwo);
}
END_TEST

START_TEST(test_centralization) {
  obj_data_t data = {0};
  obj_data_t dataCheck = {0};
  s21_create_matrix(3, 3, &data.coordinates);
  s21_create_matrix(3, 3, &dataCheck.coordinates);
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      data.coordinates.matrix[i][j] = i * j * (-1);
    }
  }
  dataCheck.coordinates.matrix[0][0] = 0;
  dataCheck.coordinates.matrix[0][1] = -1;
  dataCheck.coordinates.matrix[0][2] = -2;
  dataCheck.coordinates.matrix[1][0] = 0;
  dataCheck.coordinates.matrix[1][1] = -2;
  dataCheck.coordinates.matrix[1][2] = -4;
  dataCheck.coordinates.matrix[2][0] = 0;
  dataCheck.coordinates.matrix[2][1] = -3;
  dataCheck.coordinates.matrix[2][2] = -6;
  data.count_of_vertexes = 3;
  centralization(&data);
  for (unsigned int i = 0; i < 3; ++i) {
    for (unsigned int j = 0; j < 3; ++j) {
      ck_assert_double_eq_tol(data.coordinates.matrix[i][j],
                              dataCheck.coordinates.matrix[i][j], 1e-7);
    }
  }
  s21_remove_matrix(&data.coordinates);
  s21_remove_matrix(&dataCheck.coordinates);
}
END_TEST

START_TEST(test_projectionModel) {
  obj_data_t data = {0};
  obj_data_t dataCheck = {0};
  s21_create_matrix(2, 3, &data.coordinates);
  s21_create_matrix(2, 2, &dataCheck.coordinates);
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      data.coordinates.matrix[i][j] = i + j;
    }
  }
  dataCheck.coordinates.matrix[0][0] = 0;
  dataCheck.coordinates.matrix[0][1] = 0.833333;
  dataCheck.coordinates.matrix[1][0] = 0.833333;
  dataCheck.coordinates.matrix[1][1] = 1.66667;
  data.count_of_vertexes = 2;
  data.max_coord = 2;
  data.scale = 1;
  projectionModel(&data);
  for (unsigned int i = 0; i < 2; ++i) {
    for (unsigned int j = 0; j < 2; ++j) {
      ck_assert_double_eq_tol(data.coordinates.matrix[i][j],
                              dataCheck.coordinates.matrix[i][j], 1e-5);
    }
  }
  s21_remove_matrix(&data.coordinates);
  s21_remove_matrix(&dataCheck.coordinates);
}
END_TEST

Suite *reader_suite() {
  Suite *s1 = suite_create("Reader_suite");
  TCase *tc1 = tcase_create("Core");

  tcase_add_test(tc1, test_reader1);
  tcase_add_test(tc1, test_reader2);
  tcase_add_test(tc1, test_error_file);
  tcase_add_test(tc1, test_scale);
  tcase_add_test(tc1, test_move);
  tcase_add_test(tc1, test_rotation);
  tcase_add_test(tc1, test_matrix);
  tcase_add_test(tc1, test_set_started);
  tcase_add_test(tc1, test_max_min);
  tcase_add_test(tc1, test_copy_matrix);
  tcase_add_test(tc1, test_centralization);
  tcase_add_test(tc1, test_projectionModel);

  suite_add_tcase(s1, tc1);

  return s1;
}

int main() {
  int failed = 0;

  Suite *s1 = reader_suite();
  SRunner *runner = srunner_create(s1);

  srunner_set_fork_status(runner, CK_NOFORK);
  srunner_run_all(runner, CK_NORMAL);
  failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed == 0 ? 0 : 1;
}
