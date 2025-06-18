#include "mainwindow.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("主程序");
    setMinimumSize(400, 300);

    QLabel *welcomeLabel = new QLabel("欢迎使用本程序", this);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setFont(QFont("Arial", 16));
    setCentralWidget(welcomeLabel);
}
