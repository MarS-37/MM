#include "constants.hpp"
#include "dbmanager.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


DBManager::DBManager(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(DB_HOST());
    db.setUserName(DB_USER());
    db.setPassword(DB_PASSWORD());
}

bool DBManager::ServerAvailable()
{
    // проверяем соединение с сервером
    db.setDatabaseName("postgres");

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
    query.bindValue(":dbname", DB_NAME());

    if (query.exec() && query.next()) {
        qDebug() << "База данных" << DB_NAME() << "найдена.";

        return true;
    }

    qDebug() << "База данных не найдена.";

    return false;
}

void DBManager::CreateDatabase()
{
    QSqlQuery query;

    if (query.exec("CREATE DATABASE " + DB_NAME())) {
        qDebug() << "База данных" << DB_NAME() << "создана.";
    } else {
        qDebug() << "Ошибка создания базы данных: " << query.lastError().text();
    }
}

bool DBManager::FindTable()
{
    // подключаемся к БД
    db.setDatabaseName(DB_NAME());

    if (!db.open()) {
        qDebug() << "Ошибка подключения к базе данных: " << db.lastError().text();

        return false;
    }

    QSqlQuery query;

    query.prepare("SELECT 1 FROM information_schema.tables WHERE table_name = :tablename");
    query.bindValue(":tablename", TABLE_NAME());

    if (query.exec() && query.next()) {
        qDebug() << "Таблица" << TABLE_NAME() << "найдена.";

        return true;
    }

    qDebug() << "Таблица" << TABLE_NAME() << " не найдена.";


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
                                   ).arg(TABLE_NAME());

    if (query.exec(createTableQuery)) {
        qDebug() << "Таблица" << TABLE_NAME() << "создана.";
        // вставляем тестовые данные
        query.exec("INSERT INTO " + TABLE_NAME() + " (name, size, created_at, information) "
                                                 "VALUES ('Test1', 100, NOW(), 'Описание теста 1')");
        query.exec("INSERT INTO " + TABLE_NAME() + " (name, size, created_at, information) "
                                                 "VALUES ('Test2', 200, NOW(), 'Описание теста 2')");
    } else {
        qDebug() << "Ошибка создания таблицы: " << query.lastError().text();
    }
}

void DBManager::TablePrint()
{
    QSqlQuery query;
    query.prepare("SELECT * FROM " + TABLE_NAME());

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
