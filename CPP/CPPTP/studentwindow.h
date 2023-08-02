//
// Created by SkyRain on 2023/04/24.
//

#ifndef CXXTP_STUDENTWINDOW_H
#define CXXTP_STUDENTWINDOW_H

#include <QWidget>
#include "SList.h"
#include "classcard.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class studentwindow;
}
QT_END_NAMESPACE

class studentwindow : public QWidget {
  Q_OBJECT

public:
  explicit studentwindow(const QString&, QWidget *parent = nullptr);
  ~studentwindow() override;

private:
  Ui::studentwindow *ui;
  QString username;
  QSettings* settings;
  CXXTP::SList<int> list;
  friend CXXTP::ClassCard;
  void electClass(int id, CXXTP::ClassCard*);
  CXXTP::SList<CXXTP::Lesson*>* selectedList;
  void save();
  void refresh();
private slots:
  void saveData();
  void exit();
  void clear();
  void showMenu(const QPoint& point);
};

#endif // CXXTP_STUDENTWINDOW_H
