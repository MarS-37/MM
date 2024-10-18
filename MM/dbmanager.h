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
    bool FindTable();
    void CreateTable();
    void TablePrint();

private:
    QSqlDatabase db;
};


#endif // DBMANAGER_H
