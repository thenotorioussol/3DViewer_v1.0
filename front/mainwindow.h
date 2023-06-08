#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QTimer>
#include <QWheelEvent>

#include "../GIF/qgifimage.h"
#include "settings.h"
#include "widget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  Widget *OGLWidget;
  ~MainWindow();

 private slots:
  void on_slider_move_x_valueChanged(int value);
  void on_slider_move_y_valueChanged(int value);
  void on_slider_move_z_valueChanged(int value);
  void on_slider_rotate_x_valueChanged(int value);
  void on_slider_rotate_y_valueChanged(int value);
  void on_slider_rotate_z_valueChanged(int value);
  void on_slider_scale_valueChanged(int value);
  void ClearField();
  void converting(obj_data_t *obj_data);
  void setDefault();

  void on_actionOpen_triggered();
  void on_actionSettings_triggered();
  void on_pushButton_clicked();

  void on_save_file_clicked();
  void create_gif();
  void take_screen();
  void save_gif();

  void gif_changeStyleRed();

 private:
  Ui::MainWindow *ui;
  Settings settings;
  obj_data_t obj_data = {0};
  matrix_t coords_copy;
  bool is_alloc;
  QTimer *timer;
  QVector<QImage> gif;
  double counter = 0.0;
  void keyPressEvent(QKeyEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  QPoint mousePos;

 signals:
  void signal(int);
};
#endif  // MAINWINDOW_H
