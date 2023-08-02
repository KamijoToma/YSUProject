//
// Created by SkyRain on 2023/04/17.
//

#ifndef CXXTP_LOGINWINDOW_H
#define CXXTP_LOGINWINDOW_H

#include <QWidget>

namespace CXXTP {
QT_BEGIN_NAMESPACE
namespace Ui {
class LoginWindow;
}
QT_END_NAMESPACE

class LoginWindow : public QWidget {
  Q_OBJECT

public:
  explicit LoginWindow(QWidget *parent = nullptr);
  ~LoginWindow() override;
private:
  Ui::LoginWindow *ui;
private slots:
  void login();
  void reg();
};
} // namespace CXXTP

#endif // CXXTP_LOGINWINDOW_H
