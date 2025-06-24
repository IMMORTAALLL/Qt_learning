// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "sidebarwidget.h"
#include "homepage.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    HomePage* getHomePage() const;

private slots:
    void onSidebarItemSelected(int index);

private:
    void setupUI();
    void setupContentPages();

    SidebarWidget *sidebarWidget;
    QStackedWidget *contentStack;
};

#endif // MAINWINDOW_H
