#ifndef COSTCALCULATIONPAGE_H
#define COSTCALCULATIONPAGE_H

#include "contentwidget.h"
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QDateEdit>

class CostCalculationPage : public ContentWidget
{
    Q_OBJECT
public:
    explicit CostCalculationPage(const QString &title, QWidget *parent = nullptr);
    ~CostCalculationPage();

private slots:
    void onRecordButtonClicked();

private:
    void setupUI();
    void loadTripPlans();
    void loadCostRecords();

    QComboBox *tripComboBox;
    QLineEdit *costEdit;
    QLineEdit *usageEdit;
    QPushButton *recordButton;
    QTableWidget *costTable;
    QDateEdit *costDateEdit;
};

#endif // COSTCALCULATIONPAGE_H
