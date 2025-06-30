// ragquestionpage.cpp
#include "ragquestionpage.h"
#include <QScrollBar>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QFile>
#include <QTextStream>

RagQuestionPage::RagQuestionPage(const QString &title, QWidget *parent) : ContentWidget(title, parent)
{
    setupUI();

    // 初始化网络管理器
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &RagQuestionPage::onApiResponse);
}

RagQuestionPage::~RagQuestionPage()
{
}

void RagQuestionPage::setupUI()
{
    // 创建主内容部件
    QWidget *contentWidget = new QWidget(this);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(50, 50, 50, 50);
    contentLayout->setSpacing(30);

    // 聊天显示区域
    chatDisplay = new QTextEdit(contentWidget);
    chatDisplay->setReadOnly(true);
    chatDisplay->setStyleSheet(R"(
        QTextEdit {
            border: none;
            background-color: #F8FAFC;
            padding: 20px;
            font-size: 24px;
        }
    )");

    // 输入区域
    QWidget *inputWidget = new QWidget(contentWidget);
    inputWidget->setStyleSheet("QWidget { background-color: white; padding: 16px 24px; }");

    QHBoxLayout *inputLayout = new QHBoxLayout(inputWidget);
    inputLayout->setContentsMargins(0, 0, 0, 0);
    inputLayout->setSpacing(10);

    questionInput = new QLineEdit(inputWidget);
    questionInput->setPlaceholderText("输入您的问题...");
    questionInput->setStyleSheet(R"(
        QLineEdit {
            border: 1px solid #D1D5DB;
            border-radius: 24px;
            padding: 0 16px;
            font-size: 24px;
            height: 48px;
            background-color: #F8FAFC;
        }
        QLineEdit:focus {
            border-color: #4285F4;
            outline: none;
        }
    )");

    sendButton = new QPushButton("发送", inputWidget);
    sendButton->setStyleSheet(R"(
        QPushButton {
            background-color: #4285F4;
            color: white;
            font-size: 24px;
            border: none;
            border-radius: 24px;
            padding: 0 24px;
            height: 48px;
        }
        QPushButton:hover {
            background-color: #3367D6;
        }
        QPushButton:pressed {
            background-color: #1F4590;
        }
    )");
    connect(sendButton, &QPushButton::clicked, this, &RagQuestionPage::onSendButtonClicked);

    uploadButton = new QPushButton("上传信息", inputWidget);
    uploadButton->setStyleSheet(R"(
        QPushButton {
            background-color: #4285F4;
            color: white;
            font-size: 24px;
            border: none;
            border-radius: 24px;
            padding: 0 24px;
            height: 48px;
        }
        QPushButton:hover {
            background-color: #3367D6;
        }
        QPushButton:pressed {
            background-color: #1F4590;
        }
    )");
    connect(uploadButton, &QPushButton::clicked, this, &RagQuestionPage::onUploadButtonClicked);

    inputLayout->addWidget(questionInput, 1);
    inputLayout->addWidget(sendButton);
    inputLayout->addWidget(uploadButton);

    // 添加到主布局
    contentLayout->addWidget(chatDisplay, 1);
    contentLayout->addWidget(inputWidget);

    // 设置为内容部件
    setContentWidget(contentWidget);
}

void RagQuestionPage::addMessage(const QString &sender, const QString &message)
{
    QString contentHtml;

    if (sender == "AI助手") {
        // AI回复：处理Markdown语法
        QTextDocument mdDoc;
        mdDoc.setMarkdown(message);  // Markdown → HTML
        contentHtml = mdDoc.toHtml();
    } else {
        // 用户消息：纯文本转义（避免HTML注入）
        contentHtml = message.toHtmlEscaped();
        contentHtml = QString("<p>%1</p>").arg(contentHtml);
    }

    // 聊天气泡样式
    QString bubbleHtml;
    if (sender == "用户") {
        bubbleHtml = QString(R"(
            <div style="margin-bottom: 15px;">
                <br>
                <div style="text-align: left;">
                    <div style="display: inline-block; background-color: white; color: #1E293B;
                                border-radius: 18px; padding: 10px 16px; max-width: 80%;">
                        %1
                    </div>
                </div>
            </div>
        )").arg(contentHtml);
    } else {
        bubbleHtml = QString(R"(
            <div style="margin-bottom: 15px; text-align: left;">
                <div style="display: inline-block; background-color: white; color: #1E293B;
                            border-radius: 18px; padding: 10px 16px; max-width: 80%;
                            box-shadow: 0 2px 5px rgba(0,0,0,0.1);">
                    %1
                </div>
            </div>
        )").arg(contentHtml);
    }

    // 插入富文本（避免覆盖原有内容）
    QTextCursor cursor = chatDisplay->textCursor();
    cursor.movePosition(QTextCursor::End);  // 定位到末尾
    cursor.insertHtml(bubbleHtml);          // 插入HTML
    chatDisplay->setTextCursor(cursor);     // 更新光标（触发滚动）

    // 滚动到底部
    QScrollBar *scrollBar = chatDisplay->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void RagQuestionPage::onSendButtonClicked()
{
    QString question = questionInput->text();
    if (!question.isEmpty()) {
        addMessage("用户", question);

        // 发送问题到后端
        QUrl url("http://127.0.0.1:8000/ask");
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        QJsonObject json;
        json["question"] = question;
        QJsonDocument doc(json);
        QByteArray data = doc.toJson();

        networkManager->post(request, data);

        questionInput->clear();
    }
}

void RagQuestionPage::onUploadButtonClicked()
{
    QString inputText = questionInput->text();
    if (!inputText.isEmpty()) {
        QFile file("C:\\Users\\LENOVO\\Desktop\\zhuanyeshixun\\qt\\login\\pybackend\\simpleRAG\\data\\test.txt");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << inputText << "\n";
            file.close();
            questionInput->clear();
        }
    }
}

void RagQuestionPage::onApiResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();
        QString answer = jsonObj["answer"].toString();
        addMessage("AI助手", answer);
    } else {
        qDebug() << "请求出错:" << reply->errorString();
    }
    reply->deleteLater();
}
