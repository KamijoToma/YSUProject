#include "ui_mainwindow.h"

#include <QApplication>
#include <QDialog>
#include <QLocale>
#include <QTranslator>
#include "SSetting.h"
#include "loginwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "CXXTP_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

//    CXXTP::MyMainWindow mainWindow;
//    mainWindow.show();

    CXXTP::SSetting::init();
    CXXTP::LoginWindow w;
    w.show();
    auto r = a.exec();
    CXXTP::SSetting::save();
    return r;
}
