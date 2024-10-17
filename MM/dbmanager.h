#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class DBManager : public QObject
{
    Q_OBJECT

public:
    DBManager(QObject *parent = nullptr);
    bool isServerAvailable();
    bool FindDatabase();
    void CreateDatabase();
    bool FindTable();
    void CreateTable();
    void MyTablePrint();

private:
    QSqlDatabase db;
    QString dbName = "mm";
    QString tableName = "settings";
};

#endif // DBMANAGER_H
