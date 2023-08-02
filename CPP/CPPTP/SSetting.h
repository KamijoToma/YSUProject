//
// Created by SkyRain on 2023/05/03.
//

#ifndef CXXTP_SSETTING_H
#define CXXTP_SSETTING_H
#include "QString"
#include "QSettings"
#include "SList.h"
namespace CXXTP {
enum AuthResult{
  ADMIN,
  STUDENT,
  ERROR
};
struct User{
  QString username;
  QString password;
  bool operator==(const User&) const;
};
struct Lesson{
  QString name;
  QString teacherName;
  QString classroomName;
  unsigned int classCapacity;
  QString week;
  QString time;
  QString description;
};

class SSetting {
public:
  /**
   * Auth user with username and password
   * @param username Username
   * @param password Password
   * @return One enum in AuthResult indicating auth result.
   */
  static AuthResult authUser(const QString& username, const QString& password);
  /**
   * Add user to database
   * @param username Username
   * @param password Password
   */
  static void addUser(const QString& username, const QString& password);
  /**
   * Explicitly save data to file
   */
  static void save();
  /**
   * Get lession list pointer
   * @return Lession list
   */
  static CXXTP::SList<CXXTP::Lesson*>* getLessonList();
  /**
   * Init settings. It should be call only on application init.
   */
  static void init();
  /**
   * Get QSettings pointer. It is used in studentwindow for user data reading.
   * @return
   */
  static QSettings* getGlobalSettings();
  /**
   * Default constructor
   */
  SSetting() = default;
};
}
#endif // CXXTP_SSETTING_H
