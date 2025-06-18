#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class RegisterWindow;

class LoginWindow : public QWidget {
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:
    void loginSuccess();

private slots:
    void onLoginButtonClicked();
    void onRegisterButtonClicked();

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *registerButton;
    RegisterWindow *registerWindow;
    Ui::LoginWindow *ui;
    void paintEvent(QPaintEvent* event) override;
};



#endif // LOGINWINDOW_H
