#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QObject>


class DBManager : public QObject
{
    Q_OBJECT
public:
    explicit DBManager(QObject *parent = nullptr);

    bool ServerAvailable();
    bool FindDatabase();
    void CreateDatabase();
    bool FindTable(const QString &tableName);
    void CreateTable(const QString &tableName);
    void EnsureTablesExist();
    void TablePrint(const QString &tableName);

    // метод для добавления записей
    void AddRecords(const QString &tableName, int count);

private:
    QSqlDatabase db;
};

#endif // DBMANAGER_H
