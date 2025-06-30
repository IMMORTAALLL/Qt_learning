// ragquestionpage.h
#ifndef RAGQUESTIONPAGE_H
#define RAGQUESTIONPAGE_H

#include "contentwidget.h"
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLabel>

class RagQuestionPage : public ContentWidget
{
    Q_OBJECT
public:
    explicit RagQuestionPage(const QString &title, QWidget *parent = nullptr);
    ~RagQuestionPage();

private slots:
    void onSendButtonClicked();
    void onUploadButtonClicked();
    void onApiResponse(QNetworkReply *reply);

private:
    void setupUI();
    void addMessage(const QString &sender, const QString &message);

    QTextEdit *chatDisplay;
    QLineEdit *questionInput;
    QPushButton *sendButton;
    QPushButton *uploadButton;
    QNetworkAccessManager *networkManager;
};

#endif // RAGQUESTIONPAGE_H
