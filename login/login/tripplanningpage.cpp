// tripplanningpage.cpp
#include "tripplanningpage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

TripPlanningPage::TripPlanningPage(QWidget *parent) : ContentWidget("行程规划", parent)
{
    setupUI();
}

TripPlanningPage::~TripPlanningPage()
{
}

void TripPlanningPage::setupUI()
{
    // 创建主内容部件
    QWidget *contentWidget = new QWidget(this);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(50, 50, 50, 50);
    contentLayout->setSpacing(30);

    // 设置蓝白配色方案和增大字体
    QString style = R"(
        QWidget {
            background-color: #F8FAFC;
            color: #1E293B;
            font-size: 32px;
        }
        QLineEdit {
            border: 1px solid #D1D5DB;
            border-radius: 8px;
            padding: 0 16px;
            height: 48px;
        }
        QLineEdit:focus {
            border-color: #60A5FA;
            outline: none;
        }
        QPushButton {
            background-color: #4285F4;
            color: white;
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
        QListWidget {
            border: 1px solid #D1D5DB;
            border-radius: 8px;
        }
        QListWidget::item {
            padding: 16px;
        }
        QDateEdit {
            border: 1px solid #D1D5DB;
            border-radius: 8px;
            padding: 0 16px;
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
    )";
    contentWidget->setStyleSheet(style);

    // 输入旅行地点
    destinationEdit = new QLineEdit(contentWidget);
    destinationEdit->setPlaceholderText("输入旅行地点");

    // 选择出行日期
    dateEdit = new QDateEdit(contentWidget);
    dateEdit->setCalendarPopup(true);
    dateEdit->setDate(QDate::currentDate());

    // 添加记录按钮
    addRecordButton = new QPushButton("添加记录", contentWidget);
    connect(addRecordButton, &QPushButton::clicked, this, &TripPlanningPage::onAddRecordButtonClicked);

    // 删除记录按钮
    deleteRecordButton = new QPushButton("删除记录", contentWidget);
    connect(deleteRecordButton, &QPushButton::clicked, this, &TripPlanningPage::onDeleteRecordButtonClicked);

    // 记录列表
    recordListWidget = new QListWidget(contentWidget);

    // 布局
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(destinationEdit);
    inputLayout->addWidget(dateEdit);
    inputLayout->addWidget(addRecordButton);
    inputLayout->addWidget(deleteRecordButton);

    contentLayout->addLayout(inputLayout);
    contentLayout->addWidget(recordListWidget);

    // 设置为内容部件
    setContentWidget(contentWidget);
}

void TripPlanningPage::onAddRecordButtonClicked()
{
    QString destination = destinationEdit->text().trimmed();
    QDate date = dateEdit->date();

    if (!destination.isEmpty()) {
        QString record = QString("%1 - %2").arg(destination).arg(date.toString(Qt::ISODate));
        recordListWidget->addItem(record);
        destinationEdit->clear();
    }
}

void TripPlanningPage::onDeleteRecordButtonClicked()
{
    QList<QListWidgetItem*> selectedItems = recordListWidget->selectedItems();
    foreach (QListWidgetItem* item, selectedItems) {
        delete recordListWidget->takeItem(recordListWidget->row(item));
    }
}
