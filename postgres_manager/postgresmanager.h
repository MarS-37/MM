#ifndef POSTGRESMANAGER_H
#define POSTGRESMANAGER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>


class PostgresManager
{
public:
    PostgresManager(const QString& host, const QString& user, const QString& password, int port);
    ~PostgresManager();

    // методы для работы с базами данных
    bool createDatabase(const QString& dbName);
    bool databaseExists(const QString& dbName);
    bool dropDatabase(const QString& dbName);

    // методы для работы с таблицами
    bool createTable(const QString& dbName, const QString& createQuery);
    bool tableExists(const QString& bdName, const QString& tableName);
    bool dropTable(const QString& dbName, const QString& tableName);

private:
    QString m_host;
    QString m_user;
    QString m_password;
    int m_port;

    QSqlDatabase connectToDatabase(const QString& dbName);
    bool executeQuery(QSqlDatabase& db, const QString& query);
};
#endif // POSTGRESMANAGER_H
