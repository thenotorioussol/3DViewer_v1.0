#include "settings.h"

#include "ui_settings.h"

Settings::Settings(QWidget *parent) : QDialog(parent), ui(new Ui::Settings) {
  ui->setupUi(this);
  if (import() == 0) {
    edgesColor = QColor(50, 80, 220);
    vertexColor = QColor(160, 35, 180);
    bgColor = QColor(49, 47, 57);
    edgesType = 0;
    edgesThick = 2;
    vertexSize = 2;
    vertexDispMethod = 1;
    projectionType = 0;
  } else {
    makeStyleSheets(ui->ButtonVerticesColor, vertexColor);
    makeStyleSheets(ui->ButtonEdgesColor, edgesColor);
    makeStyleSheets(ui->ButtonBackgroundColor, bgColor);
    ui->comboBoxEdgesType->setCurrentIndex(edgesType);
    ui->comboBoxEdgesThick->setCurrentIndex(edgesThick - 1);
    ui->comboBoxVerticesSize->setCurrentIndex(vertexSize / 3 - 1);
    ui->comboBoxVerticesDispMethod->setCurrentIndex(vertexDispMethod + 1);
    ui->comboBoxProjection->setCurrentIndex(projectionType);
  }
}

Settings::~Settings() { delete ui; }

// Смена цвета ребер
void Settings::on_ButtonEdgesColor_clicked() {
  QColor color = QColorDialog::getColor(Qt::white);
  if (color.isValid()) {
    edgesColor = color;
    makeStyleSheets(ui->ButtonEdgesColor, color);
  }
}

// Смена цвета точек
void Settings::on_ButtonVerticesColor_clicked() {
  QColor color = QColorDialog::getColor(Qt::white);
  if (color.isValid()) {
    vertexColor = color;
    makeStyleSheets(ui->ButtonVerticesColor, color);
  }
}

// Смена цвета фона
void Settings::on_ButtonBackgroundColor_clicked() {
  QColor color = QColorDialog::getColor(Qt::white);
  if (color.isValid()) {
    bgColor = color;
    makeStyleSheets(ui->ButtonBackgroundColor, color);
  }
}

// Смена типа ребер
void Settings::on_comboBoxEdgesType_currentIndexChanged(int index) {
  edgesType = index;
}

// Смена толщины ребер
void Settings::on_comboBoxEdgesThick_currentIndexChanged(int index) {
  edgesThick = index + 1;
}

// Смена размера точек
void Settings::on_comboBoxVerticesSize_currentIndexChanged(int index) {
  vertexSize = (index + 1) * 3;
}

// Смена типа точек
void Settings::on_comboBoxVerticesDispMethod_currentIndexChanged(int index) {
  vertexDispMethod = index - 1;
}

// Смена типа проекции
void Settings::on_comboBoxProjection_currentIndexChanged(int index) {
  projectionType = index;
}

// Сохранение настроек
void Settings::on_SavePreferences_clicked() {
  QDir dir(QCoreApplication::applicationDirPath());
  FILE *fp = fopen(dir.filePath("cfg.txt").toUtf8().constData(), "w");
  if (fp) {
    fprintf(fp,
            "Edges color: %d %d %d\n"
            "Edges type: %d\n"
            "Edges thickness: %d\n\n"
            "Vertices color: %d %d %d\n"
            "Vertices size: %d\n"
            "Display method: %d\n\n"
            "Type of projection: %d\n"
            "Background color: %d %d %d",
            edgesColor.red(), edgesColor.green(), edgesColor.blue(), edgesType,
            edgesThick, vertexColor.red(), vertexColor.green(),
            vertexColor.blue(), vertexSize, vertexDispMethod, projectionType,
            bgColor.red(), bgColor.green(), bgColor.blue());
    fclose(fp);
  }
  this->close();
}

//
void Settings::equal(Settings &new_settings) {
  edgesColor = new_settings.edgesColor;
  vertexColor = new_settings.vertexColor;
  bgColor = new_settings.bgColor;
  edgesType = new_settings.edgesType;
  edgesThick = new_settings.edgesThick;
  vertexSize = new_settings.vertexSize;
  vertexDispMethod = new_settings.vertexDispMethod;
  projectionType = new_settings.projectionType;
}

int Settings::import() {
  QDir dir(QCoreApplication::applicationDirPath());
  FILE *fp = fopen(dir.filePath("cfg.txt").toUtf8().constData(), "r");
  int flag = 0;
  if (fp) {
    int eRed, eGreen, eBlue;
    int vRed, vGreen, vBlue;
    int bgRed, bgGreen, bgBlue;
    flag = fscanf(fp,
                  "Edges color: %d %d %d\n"
                  "Edges type: %d\n"
                  "Edges thickness: %d\n\n"
                  "Vertices color: %d %d %d\n"
                  "Vertices size: %d\n"
                  "Display method: %d\n\n"
                  "Type of projection: %d\n"
                  "Background color: %d %d %d",
                  &eRed, &eGreen, &eBlue, &edgesType, &edgesThick, &vRed,
                  &vGreen, &vBlue, &vertexSize, &vertexDispMethod,
                  &projectionType, &bgRed, &bgGreen, &bgBlue);
    edgesColor.setRgb(eRed, eGreen, eBlue);
    vertexColor.setRgb(vRed, vGreen, vBlue);
    bgColor.setRgb(bgRed, bgGreen, bgBlue);
    fclose(fp);

    if (flag != 14)
      flag = 0;
    else
      flag = 1;
  }
  return flag;
}

void Settings::makeStyleSheets(QPushButton *btn, QColor clr) {
  char buffer[50] = {};
  sprintf(buffer, "background-color: rgb(%d, %d, %d);", clr.red(), clr.green(),
          clr.blue());
  btn->setStyleSheet(buffer);
}
