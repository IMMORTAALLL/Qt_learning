#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class RegisterWindow : public QWidget {
    Q_OBJECT
public:
    explicit RegisterWindow(QWidget *parent = nullptr);

signals:
    void registrationSuccess();

private slots:
    void onRegisterButtonClicked();
    void onCancelButtonClicked();

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *confirmPasswordEdit;
    QPushButton *registerButton;
    QPushButton *cancelButton;
};

#endif // REGISTERWINDOW_H
