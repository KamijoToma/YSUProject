//
// Created by SkyRain on 2023/05/03.
//


#include "SSetting.h"
#include <iostream>
using namespace std;
static QSettings* settings;
static CXXTP::SList<CXXTP::User*>* userList;
static CXXTP::SList<CXXTP::Lesson*>* lessonList;

void CXXTP::SSetting::init() {
  settings = new QSettings("settings.ini", QSettings::IniFormat);
  userList = new SList<User*>;
  lessonList = new SList<Lesson*>;
  settings->beginGroup("User");
  auto result = settings->value("user", "").toStringList();
  settings->endGroup();
  auto lessonSize = settings->beginReadArray("Lesson");
  for(int i=0; i<lessonSize; i++){
    settings->setArrayIndex(i);
    auto item = new Lesson{
      settings->value("name", "Default").toString(),
          settings->value("teacherName", "LiMing").toString(),
          settings->value("classroomName", "Classroom1").toString(),
          settings->value("classCapacity", 0).toUInt(),
          settings->value("week", "星期一").toString(),
          settings->value("time", "第一节课").toString(),
          settings->value("description", "Default").toString()
    };
    lessonList->push_back(item);
  }
  settings->endArray();
  if(!result.empty()){
    for(const auto& item:result){
      auto k = item.split("@");
      if(k.size() != 2)
        continue ;
      auto i = new User{
          k[0],
          k[1]
      };
      qDebug() << k[0] << k[1];
      userList->push_back(i);
    }
  }
}
CXXTP::AuthResult CXXTP::SSetting::authUser(const QString &username,
                                            const QString &password) {
  if(username == "admin" && password == "admin@YSU"){
    return ADMIN;
  }
  for(int i=0; i<userList->getSize(); i++){
    auto item = (*userList)[i];
    if(username == item->username && password == item->password){
      return STUDENT;
    }
  }
  return CXXTP::ERROR;
}
void CXXTP::SSetting::addUser(const QString &username,
                              const QString &password) {
  for(int i=0; i<userList->getSize(); i++){
    auto item = (*userList)[i];
    if(item->username == username){
      item->password = password;
      return;
    }
  }
  userList->push_back(new User{username, password});
  save();
}
void CXXTP::SSetting::save() {
  QStringList userSaveList;
  for(int i=0; i<userList->getSize(); i++){
    auto item = (*userList)[i];
    userSaveList.push_back(item->username+"@"+item->password);
  }
  settings->beginGroup("User");
  settings->setValue("user", userSaveList);
  settings->endGroup();
  settings->beginWriteArray("Lesson");
  for(int i=0; i<lessonList->getSize(); i++){
    settings->setArrayIndex(i);
    auto item = (*lessonList)[i];
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
CXXTP::SList<CXXTP::Lesson *> *CXXTP::SSetting::getLessonList() {
  return lessonList;
}

QSettings *CXXTP::SSetting::getGlobalSettings() {
  return settings;
}

bool CXXTP::User::operator==(const CXXTP::User & rhs) const {
  return username == rhs.username && password == rhs.password;
}
