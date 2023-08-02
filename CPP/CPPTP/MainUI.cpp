//
// Created by SkyRain on 2023/4/5.
//
#include "MainUI.h"
#include <string>

CXXTP::MyMainWindow::MyMainWindow(QWidget* parent): QMainWindow(parent), w(), editStatus(false), index(0){
  w.setupUi(this);
  w.tabWidget->setCurrentIndex(0);
  lessonList = SSetting::getLessonList();
  refresh();
}
[[maybe_unused]] void CXXTP::MyMainWindow::addData() {
  if(!editStatus){
    auto item = new Lesson{
        w.ClassNameLineEdit->text(),
        w.nameLineEdit->text(),
        w.classroomComboBox->currentText(),
        static_cast<unsigned int>(w.ClassLimitSpinBox->value()),
        w.ClassWeekCombo->currentText(),
        w.ClassTimeCombo->currentText(),
        w.ClassDescEdit->toPlainText()
    };
    lessonList->push_back(item);
  }else{
    // Recover
    editStatus = false;
    delete (*lessonList)[index];
    (*lessonList)[index] = new Lesson{
        w.ClassNameLineEdit->text(),
        w.nameLineEdit->text(),
        w.classroomComboBox->currentText(),
        static_cast<unsigned int>(w.ClassLimitSpinBox->value()),
        w.ClassWeekCombo->currentText(),
        w.ClassTimeCombo->currentText(),
        w.ClassDescEdit->toPlainText()
    };
    w.saveButton->setText("保存");
    w.clearButton->setText("清空");
  }
  refresh();
}
void CXXTP::MyMainWindow::clearInput() {
  editStatus = false;
  w.saveButton->setText("保存");
  w.clearButton->setText("清空");
  w.ClassNameLineEdit->clear();
  w.nameLineEdit->clear();
  w.classroomComboBox->setCurrentIndex(0);
  w.ClassLimitSpinBox->setValue(0);
  w.ClassWeekCombo->setCurrentIndex(0);
  w.ClassTimeCombo->setCurrentIndex(0);
  w.ClassDescEdit->clear();
}


void CXXTP::MyMainWindow::clearData() {
}

void CXXTP::MyMainWindow::refresh() {
  auto table = w.treeWidget;
  table->clear();
  for(int i=0; i<lessonList->getSize(); i++){
    auto lesson = (*lessonList)[i];
    auto item = new QTreeWidgetItem(
        QStringList()
        << QString::number(i+1)
        << lesson->name
        << lesson->teacherName
        << lesson->classroomName
        << QString::number(lesson->classCapacity)
        << lesson->week
        << lesson->time
        << lesson->description
        );
    table->addTopLevelItem(item);
  }
  SSetting::save();
}
[[maybe_unused]] void CXXTP::MyMainWindow::saveData() {
  CXXTP::SSetting::save();
}
void CXXTP::MyMainWindow::contextMenu(const QPoint & pos) {
  auto item = w.treeWidget->itemAt(pos);
  if(item != nullptr){
    auto menu = new QMenu(this);
    auto deleteAction = new QAction("删除");
    connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(deleteData()));
    auto changeAction = new QAction("修改");
    connect(changeAction, SIGNAL(triggered(bool)), this, SLOT(edit()));
    menu->addAction(deleteAction);
    menu->addAction(changeAction);
    menu->move(mapToGlobal(pos));
    menu->show();
  }
}
void CXXTP::MyMainWindow::deleteData() {
  auto item = w.treeWidget->currentItem();
  if(item != nullptr){
    bool ok = false;
    auto index = item->text(0).toInt(&ok)-1;
    if(ok){
      lessonList->erase(index);
      refresh();
    }
  }
}
void CXXTP::MyMainWindow::edit() {
  bool ok = false;
  this->index = w.treeWidget->currentItem()->text(0).toInt(&ok)-1;
  if(!ok) return;
  w.tabWidget->setCurrentIndex(0);
  editStatus = true;
  w.saveButton->setText("确认");
  w.clearButton->setText("取消");

  // Set texts
  auto item = (*lessonList)[index];
  w.ClassNameLineEdit->setText(item->name);
  w.nameLineEdit->setText(item->teacherName);
  w.classroomComboBox->setEditText(item->classroomName);
  w.ClassLimitSpinBox->setValue(static_cast<int>(item->classCapacity));
  w.ClassWeekCombo->setCurrentText(item->week);
  w.ClassTimeCombo->setCurrentText(item->time);
  w.ClassDescEdit->setText(item->description);
}

void CXXTP::MyMainWindow::searchClear() {
  refresh();
}

void CXXTP::MyMainWindow::search() {
  _search();
  _search();
  _search();
  _search();
}
void CXXTP::MyMainWindow::_search() {
  QTreeWidgetItemIterator it(w.treeWidget);
  while(*it){
    auto item = *it;
    if(w.classnamecheckBox->isChecked() &&
        !item->text(1).contains(w.classnamelineEdit->text(), Qt::CaseSensitive)){
      delete item;
      it++;
      continue ;
    }
    if(w.teachernamecheckBox->isChecked() &&
        !item->text(2).contains(w.teachernameEdit->text(), Qt::CaseSensitive)){
      delete item;
      it++;
      continue;
    }
    if(w.timeCheckBox->isChecked()){
      if(w.weekComboBox->currentText() != "任意" && item->text(5) !=w.weekComboBox->currentText()){
        delete item;
        it++;
        continue ;
      }
      if(w.weekComboBox_2->currentText() != "任意" && item->text(6).contains(w.weekComboBox_2->currentText())){
        delete item;
        it++;
        continue ;
      }
    }
    if(w.classroomCheckBox->isChecked() &&
        item->text(3) !=w.classroomComboBox->currentText()){
      delete item;
      it++;
      continue ;
    }
    it++;
  }
}
