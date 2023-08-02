//
// Created by SkyRain on 2023/04/25.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_ClassCard.h" resolved

#include "classcard.h"
#include "ui_ClassCard.h"
#include "studentwindow.h"
namespace CXXTP {
ClassCard::ClassCard(const Lesson& lesson, const int id, QWidget *parent) : QWidget(parent), id(id), p(parent),ui(new Ui::ClassCard){
  ui->setupUi(this);
  ui->classname->setText(lesson.name);
  ui->teachername->setText(lesson.teacherName);
  ui->classtime->setText(lesson.week + " " + lesson.time);
  ui->description->setText(lesson.description);
  ui->electStatus->setText("容量 "+QString::number(lesson.classCapacity)+" 人");
}

ClassCard::~ClassCard() { delete ui; }
void ClassCard::elect() {
  auto w = dynamic_cast<studentwindow*>(p);
  w->electClass(id, this);
}
ClassCard::ClassCard() : id(0) {
  ui->setupUi(this);
}
} // namespace CXXTP
