// tripplanningpage.cpp
#include "tripplanningpage.h"
#include "userdatabase.h"
#include <QMessageBox>

TripPlanningPage::TripPlanningPage(QWidget *parent) : ContentWidget("行程记录", parent)
{
    setupUI();
    loadTrips();
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

    // 设置蓝白配色方案
    QString style = R"(
        QWidget {
            background-color: #F8FAFC;
            color: #1E293B;
            font-size: 28px;
        }
        QLineEdit {
            border: 1px solid #D1D5DB;
            border-radius: 8px;
            padding: 0 16px;
            height: 48px;
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
        QPushButton:disabled {
            background-color: #94A3B8;
        }
        QListWidget {
            border: 1px solid #D1D5DB;
            border-radius: 8px;
        }
        QListWidget::item {
            padding: 16px;
        }
        QGroupBox {
            border: 1px solid #D1D5DB;
            border-radius: 8px;
            padding: 10px;
        }
    )";
    contentWidget->setStyleSheet(style);

    // 旅程名称输入
    QHBoxLayout *tripNameLayout = new QHBoxLayout();
    tripNameEdit = new QLineEdit(contentWidget);
    tripNameEdit->setPlaceholderText("输入旅程名称");
    newTripButton = new QPushButton("新建旅程", contentWidget);
    saveTripButton = new QPushButton("保存旅程", contentWidget);
    saveTripButton->setEnabled(false);

    tripNameLayout->addWidget(tripNameEdit);
    tripNameLayout->addWidget(newTripButton);
    tripNameLayout->addWidget(saveTripButton);

    // 地点和时间输入
    QHBoxLayout *locationInputLayout = new QHBoxLayout();
    destinationEdit = new QLineEdit(contentWidget);
    destinationEdit->setPlaceholderText("输入地点");
    dateEdit = new QDateEdit(contentWidget);
    dateEdit->setCalendarPopup(true);
    dateEdit->setDate(QDate::currentDate());
    addLocationButton = new QPushButton("添加地点", contentWidget);
    addLocationButton->setEnabled(false);
    deleteLocationButton = new QPushButton("删除选中地点", contentWidget);
    deleteLocationButton->setEnabled(false);

    locationInputLayout->addWidget(destinationEdit);
    locationInputLayout->addWidget(dateEdit);
    locationInputLayout->addWidget(addLocationButton);
    locationInputLayout->addWidget(deleteLocationButton);

    // 旅程列表
    tripListWidget = new QListWidget(contentWidget);

    // 地点列表
    locationGroupBox = new QGroupBox("地点列表", contentWidget);
    QVBoxLayout *locationBoxLayout = new QVBoxLayout(locationGroupBox);
    locationListWidget = new QListWidget(contentWidget);
    locationBoxLayout->addWidget(locationListWidget);
    locationGroupBox->setVisible(false);

    // 连接信号槽
    connect(newTripButton, &QPushButton::clicked, this, &TripPlanningPage::onNewTripButtonClicked);
    connect(saveTripButton, &QPushButton::clicked, this, &TripPlanningPage::onSaveTripButtonClicked);
    connect(addLocationButton, &QPushButton::clicked, this, &TripPlanningPage::onAddLocationButtonClicked);
    connect(deleteLocationButton, &QPushButton::clicked, this, &TripPlanningPage::onDeleteLocationButtonClicked);
    connect(tripListWidget, &QListWidget::itemClicked, [this](QListWidgetItem *item) {
        currentTripName = item->text();
        updateTripDisplay(currentTripName);
    });
    connect(locationListWidget, &QListWidget::itemSelectionChanged, [this]() {
        deleteLocationButton->setEnabled(!locationListWidget->selectedItems().isEmpty());
    });

    // 添加到主布局
    contentLayout->addLayout(tripNameLayout);
    contentLayout->addLayout(locationInputLayout);
    contentLayout->addWidget(tripListWidget);
    contentLayout->addWidget(locationGroupBox);

    // 设置为内容部件
    setContentWidget(contentWidget);
}

void TripPlanningPage::loadTrips()
{
    tripListWidget->clear();

    // 从数据库加载已保存的旅程
    QSqlQuery query("SELECT DISTINCT trip_total_name FROM trip_plans");
    while (query.next()) {
        QString tripName = query.value(0).toString();
        tripListWidget->addItem(tripName);
    }

    // 添加临时旅程（未保存到数据库的）
    foreach (const QString &tripName, tempTrips.keys()) {
        if (!tempTrips[tripName].isEmpty() && !tripListWidget->findItems(tripName, Qt::MatchExactly).isEmpty()) {
            tripListWidget->addItem(tripName);
        }
    }
}

void TripPlanningPage::updateTripDisplay(const QString &tripName)
{
    locationListWidget->clear();
    locationGroupBox->setTitle(QString("地点列表 - %1").arg(tripName));
    locationGroupBox->setVisible(true);

    // 显示当前旅程的地点
    if (tempTrips.contains(tripName)) {
        foreach (const auto &location, tempTrips[tripName]) {
            locationListWidget->addItem(QString("%1 - %2").arg(location.first).arg(location.second.toString("yyyy/MM/dd")));
        }
    } else {
        // 从数据库加载
        QList<TripLocation> locations = UserDatabase::instance().getTripLocations(tripName);
        foreach (const auto &location, locations) {
            locationListWidget->addItem(QString("%1 - %2").arg(location.destination).arg(location.date.toString("yyyy/MM/dd")));
        }
    }

    // 启用相关按钮
    addLocationButton->setEnabled(true);
    saveTripButton->setEnabled(tempTrips.contains(tripName));
    tripNameEdit->setText(tripName);
    currentTripName = tripName;
}

void TripPlanningPage::onNewTripButtonClicked()
{
    QString tripName = tripNameEdit->text();
    if (tripName.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入旅程名称");
        return;
    }

    // 检查是否已存在
    if (tempTrips.contains(tripName) ||
        UserDatabase::instance().getTripLocations(tripName).size() > 0) {
        QMessageBox::warning(this, "提示", "该旅程名称已存在");
        return;
    }

    // 创建新旅程
    tempTrips[tripName] = QList<QPair<QString, QDate>>();
    tripListWidget->addItem(tripName);
    currentTripName = tripName;
    updateTripDisplay(tripName);
}

void TripPlanningPage::onAddLocationButtonClicked()
{
    if (currentTripName.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先选择或创建旅程");
        return;
    }

    QString destination = destinationEdit->text();
    QDate date = dateEdit->date();

    if (destination.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入地点名称");
        return;
    }

    // 添加到临时旅程
    tempTrips[currentTripName].append(qMakePair(destination, date));
    locationListWidget->addItem(QString("%1 - %2").arg(destination).arg(date.toString("yyyy/MM/dd")));
    destinationEdit->clear();

    // 启用保存按钮
    saveTripButton->setEnabled(true);
}

void TripPlanningPage::onSaveTripButtonClicked()
{
    if (currentTripName.isEmpty() || !tempTrips.contains(currentTripName)) {
        return;
    }

    // 保存到数据库
    if (UserDatabase::instance().addTripLocations(currentTripName, tempTrips[currentTripName])) {
        QMessageBox::information(this, "成功", "旅程已保存");
        tempTrips.remove(currentTripName);
        loadTrips();
        updateTripDisplay(currentTripName);
    } else {
        QMessageBox::warning(this, "失败", "保存旅程时出错");
    }
}

void TripPlanningPage::onDeleteLocationButtonClicked()
{
    QList<QListWidgetItem*> selectedItems = locationListWidget->selectedItems();
    if (selectedItems.isEmpty()) {
        return;
    }

    foreach (QListWidgetItem *item, selectedItems) {
        QString text = item->text();
        QStringList parts = text.split(" - ");
        if (parts.size() == 2) {
            QString destination = parts[0];
            QDate date = QDate::fromString(parts[1], "yyyy/MM/dd");

            // 如果是已保存的旅程，从数据库删除
            if (!tempTrips.contains(currentTripName)) {
                UserDatabase::instance().deleteLocation(currentTripName, destination, date);
            } else {
                // 从临时数据中删除
                for (int i = 0; i < tempTrips[currentTripName].size(); i++) {
                    if (tempTrips[currentTripName][i].first == destination &&
                        tempTrips[currentTripName][i].second == date) {
                        tempTrips[currentTripName].removeAt(i);
                        break;
                    }
                }
            }
        }

        delete locationListWidget->takeItem(locationListWidget->row(item));
    }

    // 如果地点列表为空，更新界面状态
    if (locationListWidget->count() == 0) {
        if (!tempTrips.contains(currentTripName)) {
            // 从数据库删除了所有地点，从列表中移除该旅程
            loadTrips();
            locationGroupBox->setVisible(false);
            currentTripName = "";
            addLocationButton->setEnabled(false);
            saveTripButton->setEnabled(false);
        }
    }
}
