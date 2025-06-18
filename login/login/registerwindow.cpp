#include "registerwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "userdatabase.h"

RegisterWindow::RegisterWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("用户注册");
    setFixedSize(350, 350);
    setWindowModality(Qt::ApplicationModal);

    QLabel *titleLabel = new QLabel("创建新账户", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFont(QFont("Arial", 16, QFont::Bold));

    QLabel *usernameLabel = new QLabel("用户名:", this);
    QLabel *passwordLabel = new QLabel("密码:", this);
    QLabel *confirmLabel = new QLabel("确认密码:", this);

    usernameEdit = new QLineEdit(this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit = new QLineEdit(this);
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);

    registerButton = new QPushButton("注册", this);
    registerButton->setDefault(true);
    registerButton->setMinimumHeight(35);

    cancelButton = new QPushButton("取消", this);
    cancelButton->setMinimumHeight(35);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout;

    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameEdit);
    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordEdit);
    mainLayout->addWidget(confirmLabel);
    mainLayout->addWidget(confirmPasswordEdit);
    mainLayout->addSpacing(20);

    buttonLayout->addWidget(registerButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->setSpacing(15);

    mainLayout->addLayout(buttonLayout);

    connect(registerButton, &QPushButton::clicked, this, &RegisterWindow::onRegisterButtonClicked);
    connect(cancelButton, &QPushButton::clicked, this, &RegisterWindow::onCancelButtonClicked);
}

void RegisterWindow::onRegisterButtonClicked() {
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();
    QString confirmPassword = confirmPasswordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "注册失败", "用户名和密码不能为空");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "注册失败", "两次输入的密码不一致");
        return;
    }

    if (UserDatabase::instance().registerUser(username, password)) {
        QMessageBox::information(this, "注册成功", "您已成功注册账户");
        emit registrationSuccess();
        close();
    } else {
        QMessageBox::warning(this, "注册失败", "用户名已存在，请选择其他用户名");
    }
}

void RegisterWindow::onCancelButtonClicked() {
    close();
}
