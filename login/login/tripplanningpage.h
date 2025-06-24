// tripplanningpage.h
#ifndef TRIPPLANNINGPAGE_H
#define TRIPPLANNINGPAGE_H

#include "contentwidget.h"
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QDateEdit>

class TripPlanningPage : public ContentWidget
{
    Q_OBJECT
public:
    explicit TripPlanningPage(QWidget *parent = nullptr);
    ~TripPlanningPage();

private slots:
    void onAddRecordButtonClicked();
    void onDeleteRecordButtonClicked();

private:
    void setupUI();

    QLineEdit *destinationEdit;
    QDateEdit *dateEdit;
    QPushButton *addRecordButton;
    QPushButton *deleteRecordButton;
    QListWidget *recordListWidget;
};

#endif // TRIPPLANNINGPAGE_H
