#include <QApplication>
#include "loginwindow.h"
#include "registerwindow.h"
#include "mainwindow.h"
#include "userdatabase.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    if (!UserDatabase::instance().initDatabase()) {
        return 1;
    }

    LoginWindow loginWindow;
    RegisterWindow registerWindow;
    MainWindow mainWindow;

    QObject::connect(&loginWindow, &LoginWindow::loginSuccess, &mainWindow, &MainWindow::show);

    loginWindow.show();
    return a.exec();
}
