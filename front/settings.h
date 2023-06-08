#ifndef SETTINGS_H
#define SETTINGS_H

#include <QColorDialog>
#include <QCoreApplication>
#include <QDialog>
#include <QDir>
#include <fstream>
#include <iostream>

namespace Ui {
class Settings;
}

class Settings : public QDialog {
  Q_OBJECT

 public:
  explicit Settings(QWidget *parent = nullptr);

  QColor edgesColor, vertexColor, bgColor;
  int edgesType, edgesThick;
  int vertexSize, vertexDispMethod;
  int projectionType;

  void equal(Settings &new_settings);
  ~Settings();

 private slots:

  void on_ButtonEdgesColor_clicked();
  void on_comboBoxEdgesType_currentIndexChanged(int index);
  void on_comboBoxEdgesThick_currentIndexChanged(int index);
  void on_ButtonVerticesColor_clicked();
  void on_comboBoxVerticesSize_currentIndexChanged(int index);
  void on_comboBoxVerticesDispMethod_currentIndexChanged(int index);
  void on_comboBoxProjection_currentIndexChanged(int index);
  void on_ButtonBackgroundColor_clicked();

  void on_SavePreferences_clicked();

 private:
  Ui::Settings *ui;
  int import();
  void makeStyleSheets(QPushButton *btn, QColor clr);
};

#endif  // SETTINGS_H
