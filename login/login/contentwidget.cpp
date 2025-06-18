// contentwidget.cpp
#include "contentwidget.h"

ContentWidget::ContentWidget(const QString &title, QWidget *parent) : QWidget(parent)
{
    // 设置样式
    setStyleSheet(R"(
        QWidget {
            background-color: #F8FAFC;
        }
        QLabel#titleLabel {
            font-size: 24px;
            font-weight: bold;
            color: #1E293B;
            padding: 20px;
            border-bottom: 1px solid #E2E8F0;
        }
    )");

    // 创建主布局
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 创建标题标签
    titleLabel = new QLabel(title, this);
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // 添加到布局
    mainLayout->addWidget(titleLabel);
}

ContentWidget::~ContentWidget()
{
}

void ContentWidget::setContentWidget(QWidget *widget)
{
    if (mainLayout->count() > 1) {
        // 移除现有内容
        QLayoutItem *item = mainLayout->takeAt(1);
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    // 添加新内容
    mainLayout->addWidget(widget, 1);
}
