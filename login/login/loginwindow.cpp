#include "loginwindow.h"
#include "registerwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFormLayout>
#include <QPixmap>
#include <QStyleOption>
#include <QPainter>
#include "userdatabase.h"

LoginWindow::LoginWindow(QWidget *parent) : QWidget(parent) {
    this->setObjectName("LoginWindow");


        setWindowTitle("用户登录");
        setFixedSize(1500, 1000); // 固定窗口尺寸

        // ========== 1. 创建控件 ==========
        // 标题
        QLabel *titleLabel = new QLabel("欢迎您登录AI旅行助手！", this);
        titleLabel->setFont(QFont("KaiTi", 36, QFont::Bold));
        titleLabel->setAlignment(Qt::AlignCenter);

        // 表单（用户名+密码）
        QLabel *usernameLabel = new QLabel("用户名:", this);
        QLabel *passwordLabel = new QLabel("密码:", this);
        usernameLabel->setFont(QFont("KaiTi", 20, QFont::Bold));
        passwordLabel->setFont(QFont("KaiTi", 20, QFont::Bold));

        usernameEdit = new QLineEdit(this);
        passwordEdit = new QLineEdit(this);
        passwordEdit->setEchoMode(QLineEdit::Password);

        // 按钮
       // setStyleSheet(
         //           "QPushButton{font-size:500px;}"
           //         );
        loginButton = new QPushButton("登录", this);
        registerButton = new QPushButton("注册", this);
        loginButton->setFixedSize(180,90);
        registerButton->setFixedSize(180,90);


        // ========== 2. 布局设计 ==========
        // 表单布局（标签+输入框自动对齐）
        QFormLayout *formLayout = new QFormLayout();
        formLayout->setRowWrapPolicy(QFormLayout::DontWrapRows); // 禁止换行
        formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter); // 标签右对齐
        formLayout->setSpacing(12); // 行内间距
        formLayout->addRow(usernameLabel, usernameEdit);
        formLayout->addRow(passwordLabel, passwordEdit);

        // 按钮布局
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addWidget(loginButton);
        buttonLayout->addWidget(registerButton);
        buttonLayout->setSpacing(20);
        buttonLayout->setAlignment(Qt::AlignCenter); // 按钮居中

        // 主布局（垂直居中，自动分配空白）
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(300, 150, 300, 150); // 左右留空300，上下150
        mainLayout->setSpacing(30); // 控件组之间的间距

        // 布局嵌套：标题 → 表单 → 按钮
        mainLayout->addStretch(); // 顶部空白拉伸（使内容垂直居中）
        mainLayout->addWidget(titleLabel);
        mainLayout->addLayout(formLayout);
        mainLayout->addLayout(buttonLayout);
        mainLayout->addStretch(); // 底部空白拉伸

        // ========== 3. 样式美化 ==========
        // 输入框样式
        QString editStyle = R"(
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
        )";
        usernameEdit->setStyleSheet(editStyle);
        passwordEdit->setStyleSheet(editStyle);

        // 按钮样式
        QString btnStyle = R"(
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
        )";
        loginButton->setStyleSheet(btnStyle);
        registerButton->setStyleSheet(btnStyle);

        // 标题额外样式（可选）
        titleLabel->setStyleSheet("color: #1F2937;");

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterButtonClicked);



}

void LoginWindow::onLoginButtonClicked() {
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (UserDatabase::instance().validateUser(username, password)) {
        QMessageBox::information(this, "登录成功", "欢迎回来，" + username);
        emit loginSuccess();
        close();
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误");
    }
}

void LoginWindow::onRegisterButtonClicked() {
    registerWindow = new RegisterWindow;
    registerWindow->show();
    connect(registerWindow, &RegisterWindow::registrationSuccess, [this]() {
        QMessageBox::information(this, "注册成功", "请使用新账户登录");
    });
}


void LoginWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);  // 平滑缩放

        // 加载背景图片（从资源文件或文件系统）
        QPixmap background(":/new/prefix1/image_login.png");
        if (background.isNull()) {
            // 图片加载失败时绘制默认背景（灰色）
            painter.fillRect(rect(), Qt::lightGray);
            return;
        }

        // 计算图片在窗口中居中显示的位置和大小
        QRect targetRect;
        double windowRatio = (double)width() / height();
        double imageRatio = (double)background.width() / background.height();

        if (windowRatio > imageRatio) {
            // 窗口更宽，图片宽度铺满，高度自适应
            int scaledHeight = width() / imageRatio;
            targetRect = QRect(0, (height() - scaledHeight) / 2, width(), scaledHeight);
        } else {
            // 窗口更高，图片高度铺满，宽度自适应
            int scaledWidth = height() * imageRatio;
            targetRect = QRect((width() - scaledWidth) / 2, 0, scaledWidth, height());
        }

        // 绘制背景图片
        painter.drawPixmap(targetRect, background);

        // 如果需要半透明效果，可以添加一个半透明遮罩
         painter.fillRect(rect(), QColor(255, 255, 255, 150));  // 30% 透明度的黑色遮罩

        // 调用基类实现绘制子控件
        QWidget::paintEvent(event);
}
