#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include "contentwidget.h"
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLabel>
#include <QStackedWidget>

class HomePage : public ContentWidget
{
    Q_OBJECT
public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();

private slots:
    void onSendButtonClicked();
    void onTemplateQuestionClicked();
    void onApiResponse(QNetworkReply *reply);
    void onTextEditReturnPressed();

private:
    void setupUI();
    void setupQAAssistant();
    void addMessage(const QString &sender, const QString &message);
    void showLoadingIndicator();
    void hideLoadingIndicator();

    QString apiUrl;
    QString apiKey;

    bool isQaInitialized = false;
    QStackedWidget *stackedWidget;
    QWidget *qaWidget;
    QTextEdit *chatDisplay;
    QLineEdit *questionInput;
    QPushButton *sendButton;
    QScrollArea *templateQuestionsArea;
    QWidget *templateQuestionsContainer;
    QHBoxLayout *templateQuestionsLayout;  // 修正为QHBoxLayout
    QNetworkAccessManager *networkManager;
    QLabel *loadingIndicator;
};

#endif // HOMEPAGE_H
