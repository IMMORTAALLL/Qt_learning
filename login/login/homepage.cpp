#include "homepage.h"
#include <QScrollBar>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QProgressBar>

HomePage::HomePage(QWidget *parent) : ContentWidget("首页", parent)
{
    setupUI();
    setupQAAssistant();

    // 初始化网络管理器
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &HomePage::onApiResponse);
    apiUrl = "https://api.siliconflow.cn/v1/chat/completions";
    apiKey = "sk-hzybzassmtuxgljbvefjoubgkidqihtdjntbezgwgfpvamav";
}

HomePage::~HomePage()
{
}


void HomePage::setupUI()
{
    if (!isQaInitialized) {
            setupQAAssistant();
            isQaInitialized = true;
        }
    // 创建主内容部件
    QWidget *contentWidget = new QWidget(this);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(50, 50, 50, 50);
    contentLayout->setSpacing(30);

    // 添加欢迎信息（保留标题）
    QLabel *welcomeLabel = new QLabel("欢迎使用AI旅行助手", contentWidget);
    welcomeLabel->setFont(QFont("Arial", 36, QFont::Bold));
    welcomeLabel->setAlignment(Qt::AlignCenter);

    QLabel *descriptionLabel = new QLabel("探索世界各地的精彩目的地，规划您的完美旅程", contentWidget);
    descriptionLabel->setFont(QFont("Arial", 18));
    descriptionLabel->setAlignment(Qt::AlignCenter);

    // ====== 删除以下代码（四个按钮相关） ======
    // QGridLayout *quickAccessLayout = new QGridLayout();
    // ...（按钮创建和添加的代码）...
    // contentLayout->addLayout(quickAccessLayout);
    // =========================================

    // 初始化问答助手（必须先调用，否则qaWidget为空）
    setupQAAssistant();

    // 添加问答区域，拉伸因子1，占满剩余空间
    contentLayout->addWidget(qaWidget, 1);

    // 设置为内容部件
    setContentWidget(contentWidget);
}

void HomePage::setupQAAssistant()
{

    if (qaWidget) return;
    // 创建问答助手卡片
    qaWidget = new QWidget(this);
    qaWidget->setStyleSheet(R"(
        QWidget {
            background-color: white;
            border-radius: 12px;
            box-shadow: 0 4px 20px rgba(0, 0, 0, 0.1);
        }
        QLabel#chatTitle {
            font-size: 24px;
            font-weight: bold;
            color: #1E293B;
            padding: 16px 24px;
            border-bottom: 1px solid #E2E8F0;
        }
    )");

    QVBoxLayout *qaLayout = new QVBoxLayout(qaWidget);
    qaLayout->setContentsMargins(0, 0, 0, 0);
    qaLayout->setSpacing(0);

    // 聊天标题
    QLabel *chatTitle = new QLabel("旅行智能助手", qaWidget);
    chatTitle->setObjectName("chatTitle");
    chatTitle->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    // 聊天显示区域
    chatDisplay = new QTextEdit(qaWidget);
    chatDisplay->setReadOnly(true);
    chatDisplay->setStyleSheet(R"(
        QTextEdit {
            border: none;
            background-color: #F8FAFC;
            padding: 20px;
            font-size: 24px;
        }
    )");

    // 模板问题区域
    templateQuestionsArea = new QScrollArea(qaWidget);
    templateQuestionsArea->setWidgetResizable(true);
    templateQuestionsArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    templateQuestionsArea->setStyleSheet("QScrollArea { border: none; background-color: white; }");

    templateQuestionsContainer = new QWidget(templateQuestionsArea);
    templateQuestionsContainer->setStyleSheet("QWidget { background-color: white; }");

    templateQuestionsLayout = new QHBoxLayout(templateQuestionsContainer);  // 修正为QHBoxLayout
    templateQuestionsLayout->setContentsMargins(20, 10, 20, 10);
    templateQuestionsLayout->setSpacing(10);

    // 添加模板问题
    QStringList templateQuestions = {
        "帮我规划一下去北京的行程",
        "日本东京有哪些必去的景点",
        "如何办理申根签证",
        "泰国自由行需要注意什么",
        "预算5000元能去哪里旅游"
    };

    for (const QString &question : templateQuestions) {
        QPushButton *templateButton = new QPushButton(question, templateQuestionsContainer);
        templateButton->setStyleSheet(R"(
            QPushButton {
                background-color: #E8F0FE;
                color: #1F4590;
                font-size: 24px;
                border: none;
                border-radius: 20px;
                padding: 8px 16px;
                margin: 5px;
                white-space: nowrap;
            }
            QPushButton:hover {
                background-color: #DBEAFE;
            }
        )");
        templateButton->setCursor(Qt::PointingHandCursor);
        connect(templateButton, &QPushButton::clicked, this, &HomePage::onTemplateQuestionClicked);

        templateQuestionsLayout->addWidget(templateButton);
    }

    templateQuestionsLayout->addStretch();
    templateQuestionsArea->setWidget(templateQuestionsContainer);

    // 输入区域
    QWidget *inputWidget = new QWidget(qaWidget);
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
    connect(questionInput, &QLineEdit::returnPressed, this, &HomePage::onTextEditReturnPressed);

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
    connect(sendButton, &QPushButton::clicked, this, &HomePage::onSendButtonClicked);

    inputLayout->addWidget(questionInput, 1);
    inputLayout->addWidget(sendButton);

    // 加载指示器
    loadingIndicator = new QLabel(chatDisplay);
    loadingIndicator->setFixedSize(150, 50);
    loadingIndicator->setAlignment(Qt::AlignCenter);
    loadingIndicator->setStyleSheet(R"(
        QLabel {
            background-color: #4285F4;
            color: white;
            border-radius: 15px;
            font-size: 24px;
            padding: 10px;
        }
    )");
    loadingIndicator->setText("加载中...");
    loadingIndicator->hide();

    // 添加到主布局
    qaLayout->addWidget(chatTitle);
    qaLayout->addWidget(chatDisplay, 1);
    qaLayout->addWidget(templateQuestionsArea);
    qaLayout->addWidget(inputWidget);

    // 将问答助手添加到首页（修正布局获取方式）
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(this->layout());
    if (mainLayout) {
        mainLayout->insertWidget(2, qaWidget, 1);  // 插入到快速访问按钮下方
    }

    // 添加欢迎消息
    addMessage("AI助手", "您好！我是您的旅行智能助手。有什么关于旅行的问题我可以帮助您解答吗？");
}

// 其余代码保持不变...

void HomePage::addMessage(const QString &sender, const QString &message)
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
        qDebug() <<contentHtml;
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

void HomePage::onSendButtonClicked()
{
    QString question = questionInput->text().trimmed();
    if (question.isEmpty()) return;
    qDebug() << "用户输入:" << question;
    addMessage("用户", question);
    questionInput->clear();
    showLoadingIndicator();

    // 创建API请求
    QUrl url(apiUrl);
    QNetworkRequest request(url);

    // 设置请求头，包含API密钥
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(apiKey).toUtf8());

    // 构建请求JSON（根据硅基流动API格式调整）
    QJsonObject requestData;
    requestData["model"] = "deepseek-ai/DeepSeek-V3";  // 或硅基流动支持的其他模型

    // 构建消息数组
    QJsonArray messages;
    QJsonObject userMessage;
    userMessage["role"] = "user";
    userMessage["content"] = "你现在是一个专业的智能旅行问答助手，要帮助用户解决关于旅行的问题，其他问题一律回答“请和我聊聊旅行吧~”"+question;
    messages.append(userMessage);

    requestData["messages"] = messages;
    requestData["temperature"] = 0.7;

    // 发送请求
    QByteArray requestBody = QJsonDocument(requestData).toJson();
    networkManager->post(request, requestBody);
}

void HomePage::onTemplateQuestionClicked()
{
    QPushButton *senderButton = qobject_cast<QPushButton*>(sender());
    if (senderButton) {
        questionInput->setText(senderButton->text());
        onSendButtonClicked();
    }
}

void HomePage::onApiResponse(QNetworkReply *reply)
{
    hideLoadingIndicator();

    if (reply->error() != QNetworkReply::NoError) {
        addMessage("AI助手", QString("网络错误: %1").arg(reply->errorString()));
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

    if (jsonDoc.isObject()) {
        QJsonObject responseObj = jsonDoc.object();

        // 根据硅基流动API的实际响应格式解析
        if (responseObj.contains("choices") && responseObj["choices"].isArray()) {
            QJsonArray choices = responseObj["choices"].toArray();
            if (!choices.isEmpty() && choices[0].isObject()) {
                QJsonObject choice = choices[0].toObject();
                if (choice.contains("message") && choice["message"].isObject()) {
                    QJsonObject message = choice["message"].toObject();
                    if (message.contains("content") && message["content"].isString()) {
                        QString answer = message["content"].toString();
                        addMessage("AI助手", answer);
                        reply->deleteLater();
                        return;
                    }
                }
            }
        }

        // 解析失败的提示
        addMessage("AI助手", "抱歉，未能解析API响应内容。");
    } else {
        addMessage("AI助手", "抱歉，收到的响应格式不正确。");
    }

    reply->deleteLater();
}


void HomePage::updateTemplateQuestion(const QString& destination)
{
    if (templateQuestionsLayout->count() > 0) {
        QPushButton *firstButton = qobject_cast<QPushButton*>(templateQuestionsLayout->itemAt(0)->widget());
        if (firstButton) {
            firstButton->setText("帮我规划一下去" + destination + "的行程");
        }
    }
}

void HomePage::onTextEditReturnPressed()
{
    onSendButtonClicked();
}

void HomePage::showLoadingIndicator()
{
    loadingIndicator->show();
    chatDisplay->update();
}

void HomePage::hideLoadingIndicator()
{
    loadingIndicator->hide();
}
