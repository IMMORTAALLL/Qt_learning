// tripplanningpage.h
#ifndef TRIPPLANNINGPAGE_H
#define TRIPPLANNINGPAGE_H

#include "contentwidget.h"
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QDateEdit>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QMap>

class TripPlanningPage : public ContentWidget
{
    Q_OBJECT
public:
    explicit TripPlanningPage(QWidget *parent = nullptr);
    ~TripPlanningPage();

private slots:
    void onNewTripButtonClicked();
    void onAddLocationButtonClicked();
    void onSaveTripButtonClicked();
    void onDeleteLocationButtonClicked();
    //void onTripListChanged();

private:
    void setupUI();
    void loadTrips();
    void updateTripDisplay(const QString &tripName);

    QLineEdit *tripNameEdit;
    QPushButton *newTripButton;
    QPushButton *saveTripButton;

    QLineEdit *destinationEdit;
    QDateEdit *dateEdit;
    QPushButton *addLocationButton;
    QPushButton *deleteLocationButton;

    QListWidget *tripListWidget;
    QGroupBox *locationGroupBox;
    QListWidget *locationListWidget;

    QString currentTripName;
    QMap<QString, QList<QPair<QString, QDate>>> tempTrips;
};

#endif // TRIPPLANNINGPAGE_H
