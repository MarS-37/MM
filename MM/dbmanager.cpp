#include "dbmanager.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DBManager::DBManager(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setUserName("MM_user");
    db.setPassword("U4Xmg3");
}

bool DBManager::isServerAvailable()
{
    db.setDatabaseName("postgres"); // проверяем соединение с сервером
    if (!db.open()) {
        qDebug() << "Ошибка подключения к серверу PostgreSQL: " << db.lastError().text();
        return false;
    }

    return true;
}

bool DBManager::FindDatabase()
{
    QSqlQuery query;
    query.prepare("SELECT 1 FROM pg_database WHERE datname = :dbname");
    query.bindValue(":dbname", dbName);

    if (query.exec() && query.next()) {
        qDebug() << "База данных" << dbName << "найдена.";

        return true;
    }
    qDebug() << "База данных не найдена.";

    return false;
}

void DBManager::CreateDatabase()
{
    QSqlQuery query;
    if (query.exec("CREATE DATABASE " + dbName)) {
        qDebug() << "База данных" << dbName << "создана.";
    } else {
        qDebug() << "Ошибка создания базы данных: " << query.lastError().text();
    }
}

bool DBManager::FindTable()
{
    db.setDatabaseName(dbName); // подключаемся к нашей БД
    if (!db.open()) {
        qDebug() << "Ошибка подключения к базе данных: " << db.lastError().text();

        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT 1 FROM information_schema.tables WHERE table_name = :tablename");
    query.bindValue(":tablename", tableName);

    if (query.exec() && query.next()) {
        qDebug() << "Таблица" << tableName << "найдена.";

        return true;
    }
    qDebug() << "Таблица не найдена.";


    return false;
}

void DBManager::CreateTable()
{
    QSqlQuery query;
    QString createTableQuery = QString(
                                   "CREATE TABLE %1 ("
                                   "id SERIAL PRIMARY KEY, "
                                   "name VARCHAR(255), "
                                   "size INTEGER, "
                                   "\"created_at\" TIMESTAMP, "
                                   "information TEXT)"
                                   ).arg(tableName);

    if (query.exec(createTableQuery)) {
        qDebug() << "Таблица" << tableName << "создана.";
        // Вставляем тестовые данные
        query.exec("INSERT INTO " + tableName + " (name, size, created_at, information) "
                                                "VALUES ('Test1', 100, NOW(), 'Описание теста 1')");
        query.exec("INSERT INTO " + tableName + " (name, size, created_at, information) "
                                                "VALUES ('Test2', 200, NOW(), 'Описание теста 2')");
    } else {
        qDebug() << "Ошибка создания таблицы: " << query.lastError().text();
    }
}

void DBManager::MyTablePrint()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM " + tableName);

    if (query.exec()) {
        while (query.next()) {
            int id = query.value("id").toInt();
            QString name = query.value("name").toString();
            int size = query.value("size").toInt();
            QString createTime = query.value("created_at").toString();
            QString info = query.value("information").toString();
            qDebug() << id << name << size << createTime << info;
        }
    } else {
        qDebug() << "Ошибка запроса к таблице: " << query.lastError().text();
    }
}
