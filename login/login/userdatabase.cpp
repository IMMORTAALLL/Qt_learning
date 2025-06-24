#include "userdatabase.h"
#include "mainwindow.h"
#include <QDate>
#include <QApplication>

bool UserDatabase::initDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");

    if (!db.open()) {
        qDebug() << "无法打开数据库:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    // 创建用户表
    query.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE NOT NULL,"
        "password_hash TEXT NOT NULL)"
    );

    query.exec(
            "CREATE TABLE IF NOT EXISTS trip_costs ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "trip_name TEXT NOT NULL,"
            "cost REAL NOT NULL"
            "usage TEXT NOT NULL)"
        );

    // 创建旅行规划表
    return query.exec(
        "CREATE TABLE IF NOT EXISTS trip_plans ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "destination TEXT NOT NULL,"
        "date TEXT NOT NULL)"
    );
}

bool UserDatabase::registerUser(const QString& username, const QString& password) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password_hash) VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", hashPassword(password));

    if (!query.exec()) {
        QString errorText = query.lastError().text().toLower();
        if (errorText.contains("unique") || errorText.contains("constraint")) {
            qDebug() << "用户名已存在";
            qDebug() << errorText;
            return false;
        }

        qDebug() << "注册失败:" << errorText;
        return false;
    }
    return true;
}

bool UserDatabase::validateUser(const QString& username, const QString& password) {
    QSqlQuery query;
    query.prepare("SELECT password_hash FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        QString storedHash = query.value(0).toString();
        return (storedHash == hashPassword(password));
    }
    return false;
}

QString UserDatabase::hashPassword(const QString& password) {
    QByteArray hashed = QCryptographicHash::hash(
        password.toUtf8(), QCryptographicHash::Sha256
    );
    return hashed.toHex();
}

// 新增函数实现
bool UserDatabase::addTripPlan(const QString& destination, const QDate& date) {
    QSqlQuery query;
    query.prepare("INSERT INTO trip_plans (destination, date) VALUES (:destination, :date)");
    query.bindValue(":destination", destination);
    query.bindValue(":date", date.toString(Qt::ISODate));

    if (query.exec()) {
        // 获取MainWindow指针并更新首页模板问题
        MainWindow* mainWindow = qobject_cast<MainWindow*>(qApp->activeWindow());
        if (mainWindow) {
            HomePage* homePage = mainWindow->getHomePage();
            if (homePage) {
                homePage->updateTemplateQuestion(destination);
            }
        }
        return true;
    }
    return false;
}


bool UserDatabase::deleteTripPlan(const QString& destination, const QDate& date) {
    QSqlQuery query;
    query.prepare("DELETE FROM trip_plans WHERE destination = :destination AND date = :date");
    query.bindValue(":destination", destination);
    query.bindValue(":date", date.toString(Qt::ISODate));

    return query.exec();
}

QList<QPair<QString, QDate>> UserDatabase::getTripPlans() {
    QList<QPair<QString, QDate>> plans;
    QSqlQuery query("SELECT destination, date FROM trip_plans");

    while (query.next()) {
        QString destination = query.value(0).toString();
        QDate date = QDate::fromString(query.value(1).toString(), Qt::ISODate);
        plans.append(qMakePair(destination, date));
    }

    return plans;
}
