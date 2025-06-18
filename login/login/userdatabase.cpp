#include "userdatabase.h"

bool UserDatabase::initDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");

    if (!db.open()) {
        qDebug() << "无法打开数据库:" << db.lastError().text();
        return false;
    }

    QSqlQuery query;
    return query.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT UNIQUE NOT NULL,"
        "password_hash TEXT NOT NULL)"
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
