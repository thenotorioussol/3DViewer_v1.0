#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  OGLWidget = new Widget;
  ui->gridLayout_1->addWidget(OGLWidget, 0, 0);
  timer = new QTimer(this);
  connect(this, &MainWindow::signal, OGLWidget, &Widget::slot);
  connect(ui->create_gif, SIGNAL(clicked()), this, SLOT(create_gif()));
  connect(timer, SIGNAL(timeout()), this, SLOT(take_screen()));
}

MainWindow::~MainWindow() {
  data_cleaner(&obj_data);
  if (is_alloc) {
    s21_remove_matrix(&coords_copy);
    is_alloc = false;
  }
  delete ui;
  delete OGLWidget;
}

// Move

void MainWindow::keyPressEvent(QKeyEvent *event) {
  int value = 0;
  if (event->key() == Qt::Key_W) {
    ui->slider_move_y->setValue(ui->slider_move_y->value() + 4);
    on_slider_move_y_valueChanged(ui->slider_move_y->value());
  } else if (event->key() == Qt::Key_S) {
    ui->slider_move_y->setValue(ui->slider_move_y->value() - 4);
    on_slider_move_y_valueChanged(ui->slider_move_y->value());
  } else if (event->key() == Qt::Key_A) {
    ui->slider_move_x->setValue(ui->slider_move_x->value() - 4);
    on_slider_move_x_valueChanged(ui->slider_move_x->value());
  } else if (event->key() == Qt::Key_D) {
    ui->slider_move_x->setValue(ui->slider_move_x->value() + 4);
    on_slider_move_x_valueChanged(ui->slider_move_x->value());
  }
}

void MainWindow::on_slider_move_x_valueChanged(int value) {
  double arg1 = value / 100.0;
  move_coords(&obj_data, arg1 - obj_data.offset.x, 0);
  (&obj_data)->offset.x = arg1;
  OGLWidget->obj_data = obj_data;
  OGLWidget->update();
}

void MainWindow::on_slider_move_y_valueChanged(int value) {
  double arg1 = value / 100.0;
  move_coords(&obj_data, arg1 - obj_data.offset.y, 1);
  (&obj_data)->offset.y = arg1;
  OGLWidget->obj_data = obj_data;
  OGLWidget->update();
}

void MainWindow::on_slider_move_z_valueChanged(int value) {
  double arg1 = value / 100.0;
  move_coords(&obj_data, arg1 - obj_data.offset.z, 2);
  (&obj_data)->offset.z = arg1;
  OGLWidget->obj_data = obj_data;
  OGLWidget->update();
}

// Rotate

void MainWindow::mousePressEvent(QMouseEvent *event) {
  mousePos = event->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
  on_slider_rotate_x_valueChanged(event->position().y() - mousePos.y());
  ui->slider_rotate_x->setValue(event->position().y() - mousePos.y());

  on_slider_rotate_y_valueChanged(event->position().x() - mousePos.x());
  ui->slider_rotate_y->setValue(event->position().x() - mousePos.x());
}

void MainWindow::on_slider_rotate_x_valueChanged(int value) {
  double dvalue = value * M_PI / 180.0;
  rotation_x(&obj_data.coordinates, dvalue - obj_data.angle.x);
  (&obj_data)->angle.x = dvalue;
  OGLWidget->obj_data = obj_data;
  OGLWidget->update();
}

void MainWindow::on_slider_rotate_y_valueChanged(int value) {
  double dvalue = value * M_PI / 180.0;
  rotation_y(&obj_data.coordinates, dvalue - obj_data.angle.y);
  (&obj_data)->angle.y = dvalue;
  OGLWidget->obj_data = obj_data;
  OGLWidget->update();
}

void MainWindow::on_slider_rotate_z_valueChanged(int value) {
  double dvalue = value * M_PI / 180.0;
  rotation_z(&obj_data.coordinates, dvalue - obj_data.angle.z);
  (&obj_data)->angle.z = dvalue;
  OGLWidget->obj_data = obj_data;
  OGLWidget->update();
}

// Scale

void MainWindow::wheelEvent(QWheelEvent *event) {
  int offset = 0;
  if (event->angleDelta().y() > 0) {
    offset = 8;
  } else if (event->angleDelta().y() < 0) {
    offset = -8;
  }
  ui->slider_scale->setValue(ui->slider_scale->value() + offset);
}

void MainWindow::on_slider_scale_valueChanged(int value) {
  if (value == 0) value = 1;
  if (obj_data.scale == 0) obj_data.scale = 1;
  double rate = value / 20.0;
  scale(&obj_data.coordinates, rate / obj_data.scale);
  (&obj_data)->scale = rate;
  OGLWidget->obj_data = obj_data;
  OGLWidget->update();
}

// File opening
void MainWindow::on_actionOpen_triggered() {
  ClearField();
  QString filename =
      QFileDialog::getOpenFileName(this, "Upload a file", "/", "*.obj");

  if (!filename.isEmpty()) {
    ui->filename->setText("Filename: ");
    ui->vertex_cnt->setText("Count of vertex: ");
    ui->poly_cnt->setText("Count of polygons: ");

    QByteArray file_path = filename.toLocal8Bit();
    QString file =
        filename.right(filename.length() - filename.lastIndexOf('/') - 1);
    ui->filename->setText(ui->filename->text() + file);

    int flag = read_file(file_path.data(), &obj_data);
    if (flag != OK) {
      this->~MainWindow();
    }

    centralization(&obj_data);
    converting(&obj_data);

    s21_create_matrix(obj_data.coordinates.rows, 3, &coords_copy);
    s21_copy_matrix(&coords_copy, &obj_data.coordinates);
    is_alloc = true;

    ui->vertex_cnt->setText(ui->vertex_cnt->text() +
                            QString::number(obj_data.count_of_vertexes));
    ui->poly_cnt->setText(ui->poly_cnt->text() +
                          QString::number(obj_data.count_of_facets));
  }
  if (obj_data.count_of_vertexes != 0) {
    emit signal(1);
    OGLWidget->obj_data = obj_data;
    OGLWidget->update();
  }
}

// Settings menu open
void MainWindow::on_actionSettings_triggered() {
  settings.move(this->pos().x() + 300, this->pos().y() + 150);
  settings.setModal(true);
  settings.exec();
  OGLWidget->settings.equal(this->settings);
  OGLWidget->update();
}

// Other
void MainWindow::converting(obj_data_t *obj_data) {
  for (int i = 0; i < obj_data->count_of_vertexes; i++) {
    for (int j = 0; j < 3; j++) {
      obj_data->coordinates.matrix[i][j] =
          (obj_data->coordinates.matrix[i][j] - obj_data->min_coord) /
          (obj_data->max_coord - obj_data->min_coord);
      obj_data->coordinates.matrix[i][j] =
          obj_data->coordinates.matrix[i][j] * 0.7;
    }
  }
}

void MainWindow::ClearField() {
  OGLWidget->flag = 0;
  setDefault();
}

void MainWindow::on_pushButton_clicked() {
  setDefault();
  OGLWidget->obj_data = obj_data;
  OGLWidget->obj_data.coordinates = coords_copy;
  OGLWidget->update();
}

void MainWindow::setDefault() {
  ui->slider_scale->setValue(20.00);
  ui->slider_move_x->setValue(0.00);
  ui->slider_move_y->setValue(0.00);
  ui->slider_move_z->setValue(0.00);
  ui->slider_rotate_x->setValue(0.00);
  ui->slider_rotate_y->setValue(0.00);
  ui->slider_rotate_z->setValue(0.00);
}

void MainWindow::on_save_file_clicked() {
  QString save_picture = NULL;
  save_picture = QFileDialog::getSaveFileName(this, NULL, NULL,
                                              "JPEG (*.jpeg) ;; BMP (*.bmp)");
  if (!save_picture.isNull())
    OGLWidget->grabFramebuffer().save(save_picture, NULL, 100);
}

void MainWindow::create_gif() {
  gif_changeStyleRed();
  timer->start(100);
  take_screen();
  counter = 0.0;
}

void MainWindow::take_screen() {
  if (counter <= 5.0) {
    gif.push_back(OGLWidget->grabFramebuffer());
    counter += 0.1;
  } else {
    save_gif();
    timer->stop();
  }
}

void MainWindow::save_gif() {
  QString gif_name = NULL;
  gif_name = QFileDialog::getSaveFileName(this, NULL, NULL,
                                          "GIF (*.gif) ;; GIF (*.gif)");
  if (!gif_name.isNull()) {
    QGifImage gif_file(QSize(OGLWidget->width() * 2, OGLWidget->height() * 2));
    gif_file.setDefaultDelay(100);
    for (QVector<QImage>::Iterator frames = gif.begin(); frames != gif.end();
         frames++) {
      gif_file.addFrame(*frames);
    }
    gif_file.save(gif_name);
  }
  ui->create_gif->setStyleSheet(ui->save_file->styleSheet());
  gif.clear();
  counter = 0;
}

void MainWindow::gif_changeStyleRed() {
  ui->create_gif->setStyleSheet(
      "border-radius: 10px;\nborder-style: solid;\nbackground-color: rgb(220, "
      "80, 80);\ncolor: rgb(221, 231, 220);");
}
