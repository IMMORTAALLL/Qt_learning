#include "costcalculationpage.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QHeaderView>
#include "userdatabase.h"

CostCalculationPage::CostCalculationPage(const QString &title, QWidget *parent) : ContentWidget(title, parent)
{
    setupUI();
    loadTripPlans();
    loadCostRecords();
}

CostCalculationPage::~CostCalculationPage()
{
}

void CostCalculationPage::setupUI()
{
    // 创建主内容部件
    QWidget *contentWidget = new QWidget(this);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(50, 50, 50, 50);
    contentLayout->setSpacing(30);

    // 行程选择下拉框
    tripComboBox = new QComboBox(contentWidget);
    tripComboBox->setStyleSheet(R"(
        QComboBox {
            border: 1px solid #D1D5DB;
            border-radius: 8px;
            padding: 0 16px;
            font-size: 30px;
            height: 48px;
        }
        QComboBox::drop-down {
            border: none;
        }
    )");

    // 费用输入框
    costEdit = new QLineEdit(contentWidget);
    costEdit->setPlaceholderText("输入费用");
    costEdit->setStyleSheet(R"(
        QLineEdit {
            border: 1px solid #D1D5DB;
            border-radius: 8px;
            padding: 0 16px;
            font-size: 30px;
            height: 48px;
        }
        QLineEdit:focus {
            border-color: #60A5FA;
            outline: none;
        }
    )");

    // 用途输入框
    usageEdit = new QLineEdit(contentWidget);
    usageEdit->setPlaceholderText("输入用途");
    usageEdit->setStyleSheet(R"(
        QLineEdit {
            border: 1px solid #D1D5DB;
            border-radius: 8px;
            padding: 0 16px;
            font-size: 30px;
            height: 48px;
        }
        QLineEdit:focus {
            border-color: #60A5FA;
            outline: none;
        }
    )");


    costDateEdit = new QDateEdit(contentWidget);
        costDateEdit->setCalendarPopup(true);
        costDateEdit->setDate(QDate::currentDate());
        costDateEdit->setStyleSheet(R"(
            QDateEdit {
                border: 1px solid #D1D5DB;
                border-radius: 8px;
                padding: 0 16px;
                font-size: 30px;
                height: 48px;
            }
            QDateEdit::drop-down {
                subcontrol-origin: padding;
                subcontrol-position: top right;
                width: 20px;
                border-left-width: 1px;
                border-left-color: darkgray;
                border-left-style: solid;
                border-top-right-radius: 3px;
                border-bottom-right-radius: 3px;
            }
        )");

    // 记录按钮
    recordButton = new QPushButton("记录费用", contentWidget);
    recordButton->setStyleSheet(R"(
        QPushButton {
            background-color: #4285F4;
            color: white;
            font-size: 32px;
            border: none;
            border-radius: 8px;
            padding: 0 32px;
            height: 48px;
        }
        QPushButton:hover {
            background-color: #3367D6;
        }
        QPushButton:pressed {
            background-color: #1F4590;
        }
    )");
    connect(recordButton, &QPushButton::clicked, this, &CostCalculationPage::onRecordButtonClicked);

    // 费用记录表格
    costTable = new QTableWidget(contentWidget);
    costTable->setColumnCount(4);
    costTable->setHorizontalHeaderLabels({"行程名称", "费用", "用途", "消费时间"});
    costTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 添加到布局
    contentLayout->addWidget(tripComboBox);
    contentLayout->addWidget(costEdit);
    contentLayout->addWidget(usageEdit);
    contentLayout->addWidget(costDateEdit);
    contentLayout->addWidget(recordButton);
    contentLayout->addWidget(costTable);

    // 设置为内容部件
    setContentWidget(contentWidget);
}

void CostCalculationPage::loadTripPlans()
{
    QSqlQuery query;
    query.prepare("SELECT destination FROM trip_plans");
    if (query.exec()) {
        while (query.next()) {
            QString tripName = query.value(0).toString();
            tripComboBox->addItem(tripName);
        }
    } else {
        qDebug() << "Failed to load trip plans:" << query.lastError().text();
    }
}

void CostCalculationPage::loadCostRecords()
{
    QSqlQuery query;
    query.prepare("SELECT trip_name, cost, usage, date FROM trip_costs");
    if (query.exec()) {
        costTable->setRowCount(0);
        while (query.next()) {
            int row = costTable->rowCount();
            costTable->insertRow(row);

            costTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
            costTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
            costTable->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
            costTable->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
        }
    } else {
        qDebug() << "Failed to load cost records:" << query.lastError().text();
    }
}

void CostCalculationPage::onRecordButtonClicked()
{
    QString selectedTrip = tripComboBox->currentText();
    QString costStr = costEdit->text();
    QString usage = usageEdit->text();
    QString date = costDateEdit->text();

    if (selectedTrip.isEmpty() || costStr.isEmpty() || usage.isEmpty() || date.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入完整的行程、费用、用途和时间信息");
        return;
    }

    bool ok;
    double cost = costStr.toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "输入错误", "请输入有效的费用金额");
        return;
    }

    // 将费用记录插入数据库
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO trip_costs (trip_name, cost, usage, date) VALUES (:trip_name, :cost, :usage, :date)");
    insertQuery.bindValue(":trip_name", selectedTrip);
    insertQuery.bindValue(":cost", cost);
    insertQuery.bindValue(":usage", usage);
    insertQuery.bindValue(":date", date);

    if (insertQuery.exec()) {
        QMessageBox::information(this, "记录成功", "费用记录已成功添加");
        // 重新加载费用记录
        loadCostRecords();
    } else {
        qDebug() << "Failed to record cost:" << insertQuery.lastError().text();
        QMessageBox::warning(this, "记录失败", "费用记录添加失败，请稍后重试");
    }
}
