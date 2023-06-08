#include "widget.h"

Widget::Widget(QWidget *parent) : QOpenGLWidget{parent} { this->flag = 0; }

void Widget::slot(int arg1) { this->flag = arg1; }

void Widget::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

void Widget::paintGL() {
  initializeGL();
  if (settings.projectionType == 1) {
    gluPerspective(45.0f, (GLfloat)670 / (GLfloat)645, 0.0001, 1000000.0);
    glTranslatef(0, 0, -2.0f);
  }
  glClearColor(settings.bgColor.redF(), settings.bgColor.greenF(),
               settings.bgColor.blueF(), 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (flag) {
    if (settings.vertexDispMethod != -1) {
      for (int i = 0; i < obj_data.count_of_facets; i++) {
        for (int j = 0; j < obj_data.polygons[i].numbers_of_vertexes; j++) {
          DrawPoint(
              obj_data.coordinates.matrix[obj_data.polygons[i].vertexes[j]][0],
              obj_data.coordinates.matrix[obj_data.polygons[i].vertexes[j]][1],
              obj_data.coordinates.matrix[obj_data.polygons[i].vertexes[j]][2]);
        }
      }
    }
    for (int i = 0; i < obj_data.count_of_facets; i++) {
      glColor3d(settings.edgesColor.redF(), settings.edgesColor.greenF(),
                settings.edgesColor.blueF());
      glLineWidth(settings.edgesThick);
      if (settings.edgesType) {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 5);
      }
      glBegin(GL_LINE_LOOP);
      for (int j = 0; j < obj_data.polygons[i].numbers_of_vertexes; j++) {
        glVertex3d(
            obj_data.coordinates.matrix[obj_data.polygons[i].vertexes[j]][0],
            obj_data.coordinates.matrix[obj_data.polygons[i].vertexes[j]][1],
            obj_data.coordinates.matrix[obj_data.polygons[i].vertexes[j]][2]);
      }
      glEnd();
      if (settings.edgesType) glDisable(GL_LINE_STIPPLE);
    }
  }
  DrawAxis();
}

void Widget::resizeGL(int w, int h) {}

void Widget::DrawPoint(double x, double y, double z) {
  glColor3d(settings.vertexColor.redF(), settings.vertexColor.greenF(),
            settings.vertexColor.blueF());
  glEnable(GL_BLEND);
  if (settings.vertexDispMethod == 0) {
    glEnable(GL_POINT_SMOOTH);
  } else if (settings.vertexDispMethod == 1) {
    glDisable(GL_POINT_SMOOTH);
  }
  glPointSize(settings.vertexSize);
  glBegin(GL_POINTS);
  glVertex3f(x, y, z);
  glEnd();
}

void Widget::DrawAxis() {
  glLineWidth(2.0f);
  glColor4f(0, 0, 0, 0.1);
  glBegin(GL_LINES);
  glVertex3f(1.0f, 0.0f, 0.0f);
  glVertex3f(-1.0f, 0.0f, 0.0f);
  glEnd();
  glBegin(GL_LINES);
  glVertex3f(0.0f, 1.0f, 0.0f);
  glVertex3f(0.0f, -1.0f, 0.0f);
  glEnd();
  glBegin(GL_LINES);
  glVertex3f(0.0f, 0.0f, 1.0f);
  glVertex3f(0.0f, 0.0f, -1.0f);
  glEnd();
}
