// mainwindow.cpp
#include "mainwindow.h"
#include "homepage.h"
#include "tripplanningpage.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUI();
    setupContentPages();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setWindowTitle("AI旅行助手");
    setFixedSize(1500,1000);

    // 创建中央部件和主布局
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 创建侧边栏
    sidebarWidget = new SidebarWidget(this);
    sidebarWidget->setFixedWidth(220);

    // 创建内容堆栈
    contentStack = new QStackedWidget(this);

    // 添加到主布局
    mainLayout->addWidget(sidebarWidget);
    mainLayout->addWidget(contentStack, 1);

    setCentralWidget(centralWidget);

    // 连接信号和槽
    connect(sidebarWidget, &SidebarWidget::itemSelected, this, &MainWindow::onSidebarItemSelected);
}

void MainWindow::setupContentPages()
{
    // 添加各个内容页面
    contentStack->addWidget(new HomePage(this));        // 首页 - 索引0
    contentStack->addWidget(new QWidget(this));
    contentStack->addWidget(new TripPlanningPage(this));
    // 默认显示首页
    sidebarWidget->selectItem(0);
}

void MainWindow::onSidebarItemSelected(int index)
{
    // 根据选择的索引切换内容页面
    if (index >= 0 && index < contentStack->count()) {

        contentStack->setCurrentIndex(index);
    }
}
