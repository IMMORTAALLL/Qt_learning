// sidebarwidget.h
#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QIcon>

class SidebarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SidebarWidget(QWidget *parent = nullptr);
    ~SidebarWidget();

    void selectItem(int index);

signals:
    void itemSelected(int index);

private slots:
    void onListItemClicked(const QModelIndex &index);

private:
    void setupUI();

    QListWidget *listWidget;
};

#endif // SIDEBARWIDGET_H
