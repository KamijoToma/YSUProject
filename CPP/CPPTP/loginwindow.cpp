//
// Created by SkyRain on 2023/04/17.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_LoginWindow.h" resolved

#include "loginwindow.h"
#include "ui_LoginWindow.h"
#include "MainUI.h"
#include "studentwindow.h"
#include "SSetting.h"

namespace CXXTP {
LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::LoginWindow) {
  ui->setupUi(this);
}

LoginWindow::~LoginWindow() { delete ui; }
void LoginWindow::login() {
  auto username = ui->usernameField->text();
  auto password = ui->passwordField->text();
  switch(CXXTP::SSetting::authUser(username, password)){
  case ADMIN:
    this->close();
    (new MyMainWindow)->show();
    break;
  case STUDENT:
    this->close();
    (new studentwindow(username))->show();
    break;
  case ERROR:
    ui->infoLabel->setText("用户名或密码错误");
    break;
  }
}
void LoginWindow::reg() {
  auto username = ui->usernameField->text();
  auto password = ui->passwordField->text();
  CXXTP::SSetting::addUser(username, password);
  ui->infoLabel->setText("注册成功");
}
} // namespace CXXTP
