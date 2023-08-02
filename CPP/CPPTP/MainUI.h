//
// Created by SkyRain on 2023/3/22.
//

#ifndef CXXTP_MAINUI_H
#define CXXTP_MAINUI_H
#include "SList.h"
#include "SSetting.h"
#include "ui_mainwindow.h"

#include <QDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItemModel>
#include <Qt>

namespace CXXTP {

class MyMainWindow : public QMainWindow{
  Q_OBJECT
public:
  MyMainWindow(QWidget* parent= nullptr);
  ::Ui::MainWindow w{};

private:
  QStandardItemModel dataModel;
  QList<QList<QStandardItem*>*> dataList;
  CXXTP::SList<CXXTP::Lesson*>* lessonList;
  bool editStatus;
  int index;
  void refresh();
public slots:
  void slotCatch(){
    QMessageBox box(this);
    box.setWindowTitle("Click");
    box.setDetailedText("You clicked this button!");
    box.setText("You clicked the button!");
    box.exec();
  }

  void aboutSlot(){
    QMessageBox box(this);
    box.setWindowTitle("About");
    box.setDetailedText("Demo Program about QT6.0");
    box.setText("Demo Program about QT6.0\nBuild with CMake Qt6 MSVC.");
    box.exec();
  }

  [[maybe_unused]] void addData();
  void clearData();
  void clearInput();
  [[maybe_unused]] void saveData();
  void contextMenu(const QPoint&);
  void deleteData();
  void edit();
  void search();
  void _search();
  void searchClear();
};
}
#endif // CXXTP_MAINUI_H
