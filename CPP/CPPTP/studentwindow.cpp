//
// Created by SkyRain on 2023/04/24.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_studentwindow.h" resolved

#include "studentwindow.h"
#include "ui_studentwindow.h"
#include "ui_ClassCard.h"
#include "classcard.h"
#include <QMenu>
#include <QAction>

int convertWeek(const QString& week){
  if(week == "星期一")
    return 0;
  if(week == "星期二")
    return 1;
  if(week == "星期三")
    return 2;
  if(week == "星期四")
    return 3;
  if(week == "星期五")
    return 4;
  if(week == "星期六")
    return 5;
  if(week == "星期日")
    return 6;
  return 0;
}

int convertTime(const QString& time){
  if(time == "第一节课")
    return 0;
  if(time == "第二节课")
    return 1;
  if(time == "第三节课")
    return 2;
  if(time == "第四节课")
    return 3;
  if(time == "第五节课")
    return 4;
  if(time == "第六节课")
    return 5;
  if(time == "第七节课")
    return 6;
  if(time == "第八节课")
    return 7;
  return 0;
}

studentwindow::studentwindow(const QString& username, QWidget *parent)
    : QWidget(parent), ui(new Ui::studentwindow), selectedList(new CXXTP::SList<CXXTP::Lesson*>),
username(username), settings(CXXTP::SSetting::getGlobalSettings()){
  ui->setupUi(this);
  ui->usernameLabel->setText(username);
  ui->tableWidget->clearContents();
  const auto size = settings->beginReadArray("Stu"+username);
  for(int i=0; i<size; i++){
    settings->setArrayIndex(i);
    auto item = new CXXTP::Lesson{
        settings->value("name", "Default").toString(),
        settings->value("teacherName", "LiMing").toString(),
        settings->value("classroomName", "Classroom1").toString(),
        settings->value("classCapacity", 0).toUInt(),
        settings->value("week", "星期一").toString(),
        settings->value("time", "第一节课").toString(),
        settings->value("description", "Default").toString()
    };
    auto week = convertWeek(item->week);
    auto time = convertTime(item->time);
    auto tableItem = ui->tableWidget->item(time, week);
    if(tableItem == nullptr){
      auto tItem = new QTableWidgetItem(item->name);
      selectedList->push_back(item);
      ui->tableWidget->setItem(time, week, tItem);
    }
    selectedList->push_back(item);
  }
  settings->endArray();
  for(int i=0; i<CXXTP::SSetting::getLessonList()->getSize(); i++){
    auto item = new CXXTP::ClassCard(*(*CXXTP::SSetting::getLessonList())[i], i, this);
    ui->gridLayout_2->addWidget(item, i/3, i%3);
  }
}

void studentwindow::save(){
  settings->beginWriteArray("Stu"+username);
  for(int i=0; i<selectedList->getSize(); i++){
    settings->setArrayIndex(i);
    auto item = (*selectedList)[i];
    settings->setValue("name", item->name);
    settings->setValue("teacherName", item->teacherName);
    settings->setValue("classroomName", item->classroomName);
    settings->setValue("classCapacity", item->classCapacity);
    settings->setValue("week", item->week);
    settings->setValue("time", item->time);
    settings->setValue("description", item->description);
  }
  settings->endArray();
}

studentwindow::~studentwindow() {
  delete ui;
  save();
}

void studentwindow::electClass(const int id, CXXTP::ClassCard* card) {
  auto lessonList = CXXTP::SSetting::getLessonList();
  auto item = (*lessonList)[id];
  auto week = convertWeek(item->week);
  auto time = convertTime(item->time);
  auto tableItem = ui->tableWidget->item(time, week);
  if(tableItem == nullptr){
    auto tItem = new QTableWidgetItem(item->name);
    selectedList->push_back(item);
    ui->tableWidget->setItem(time, week, tItem);
    card->ui->electButton->setDisabled(true);
  }
}
void studentwindow::saveData() {
  save();
}
void studentwindow::exit() {
  QApplication* app;
  save();
  app->exit();
}
void studentwindow::clear() {
  ui->tableWidget->clearContents();
  selectedList->clear();
  for(int i=0; i<ui->gridLayout_2->count(); i++){
    auto widget = ui->gridLayout_2->itemAt(i)->widget();
    auto card = dynamic_cast<CXXTP::ClassCard*>(widget);
    card->ui->electButton->setEnabled(true);
  }
}

void studentwindow::showMenu(const QPoint &point) {
  QMenu menu(this);
  auto deleteAction = new QAction("删除", this);
  menu.addAction(deleteAction);
  menu.move(mapToGlobal(point));
  menu.show();
}
