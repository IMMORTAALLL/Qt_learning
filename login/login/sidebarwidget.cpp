// 修改后的 sidebarwidget.cpp
#include "sidebarwidget.h"
#include <QLabel>

SidebarWidget::SidebarWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

SidebarWidget::~SidebarWidget()
{
}

void SidebarWidget::setupUI()
{
    // 应用蓝白色配色方案
    setStyleSheet(R"(
        QWidget {
            background-color: #1E293B;
            color: white;
        }
        QListWidget {
            background-color: #1E293B;
            border: none;
            outline: none;
                  font-size:48px;
                  font-family:KaiTi;

        }
        QListWidget::item {
            padding: 24px 40px;
                  border-bottom: 1px solid #334155;// 增加内边距
            width: 100px;        // 增加高度
            color: #FFFFFF;
            font-size: 32px;     // 增大字体
        }
        QListWidget::item:selected {
            background-color: #4285F4;  // 主蓝色
            color: white;
            border-left: 4px solid #3367D6;  // 深一点的蓝色
        }
        QListWidget::item:hover {
            background-color: #334155;  // 悬停时背景变深
        }
    )");

    // 创建垂直布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);  // 增加顶部边距
    layout->setSpacing(0);

    // 创建标题标签
    QLabel *titleLabel = new QLabel("菜单栏", this);
    titleLabel->setFont(QFont("Arial", 24, QFont::Bold));  // 增大标题字体
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setMargin(40);  // 增加边距

    // 创建列表控件
    listWidget = new QListWidget(this);
    listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // 添加菜单项（保持原有图标不变）
    //listWidget->addItem(new QListWidgetItem(""));
    listWidget->addItem(new QListWidgetItem(QIcon(":/icons/home.png"), "智能问答"));
    listWidget->addItem(new QListWidgetItem(""));
    listWidget->addItem(new QListWidgetItem(QIcon(":/icons/trip.png"), "行程记录"));
    listWidget->addItem(new QListWidgetItem(""));
    listWidget->addItem(new QListWidgetItem(QIcon(":/icons/cost.png"), "行程计费"));
    listWidget->addItem(new QListWidgetItem(""));
    listWidget->addItem(new QListWidgetItem(QIcon(":/icons/profile.png"), "专属规划"));
    listWidget->addItem(new QListWidgetItem(""));
    listWidget->addItem(new QListWidgetItem(QIcon(":/icons/profile.png"), "旅行绘卷"));

    // 添加到布局
    layout->addWidget(titleLabel);
    layout->addWidget(listWidget, 1);

    QString listqss = R"(
            QListWidget{

            font-family:"KaiTi";
            font-size:32;

                      }
                      )";
    //listWidget->setStyleSheet(listqss);

    // 连接信号和槽
    connect(listWidget, &QListWidget::clicked, this, &SidebarWidget::onListItemClicked);
}

// 其余代码保持不变
void SidebarWidget::selectItem(int index)
{
    if (index >= 0 && index < listWidget->count()) {
        listWidget->setCurrentRow(index);
        emit itemSelected(index);
    }
}

void SidebarWidget::onListItemClicked(const QModelIndex &index)
{
    emit itemSelected(index.row());
}
