#ifndef WIDGET_H
#define WIDGET_H
#define GL_SILENCE_DEPRECATION

#include <openGL/glu.h>

#include <QOpenGLWidget>

#include "settings.h"

extern "C" {
#include "../backend/reader.h"
#include "../backend/s21_matrix.h"
#include "../backend/transformation.h"
}

class Widget : public QOpenGLWidget {
  Q_OBJECT
 public:
  explicit Widget(QWidget *parent = nullptr);

  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;
  void DrawPoint(double x, double y, double z);
  void DrawLine();
  void DrawAxis();

 public slots:
  void slot(int);

 public:
  obj_data_t obj_data = {0};
  int flag;
  Settings settings;
};

#endif  // WIDGET_H
