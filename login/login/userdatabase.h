#ifndef USERDATABASE_H
#define USERDATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDebug>

class UserDatabase {
public:
    static UserDatabase& instance() {
        static UserDatabase db;
        return db;
    }

    bool initDatabase();
    bool registerUser(const QString& username, const QString& password);
    bool validateUser(const QString& username, const QString& password);

private:
    QSqlDatabase db;

    UserDatabase() {}
    ~UserDatabase() { if (db.isOpen()) db.close(); }
    UserDatabase(const UserDatabase&) = delete;
    UserDatabase& operator=(const UserDatabase&) = delete;

    QString hashPassword(const QString& password);
};

#endif // USERDATABASE_H
