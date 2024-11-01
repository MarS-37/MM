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
    db.setPassword(DB_PASS());
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
    // проверяем соединение с сервером
    db.setDatabaseName("postgres");

    if (!db.open()) {
        qDebug() << "Ошибка подключения к серверу PostgreSQL(поиск): " << db.lastError().text();

        return false;
    }

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
    // подключаемся к серверу с базой
    // данных по умолчанию postgres
    db.setDatabaseName("postgres");

    if (!db.open()) {
        qDebug() << "Ошибка подключения к серверу для создания базы данных: " << db.lastError().text();

        return;
    }

    QSqlQuery query;

    // создаем БД
    if (query.exec("CREATE DATABASE " + DB_NAME())) {
        qDebug() << "База данных" << DB_NAME() << "создана.";
    }
    else {
        qDebug() << "Ошибка создания базы данных: " << query.lastError().text();
    }
}

bool DBManager::FindTable(const QString &tableName)
{
    // подключаемся к БД
    db.setDatabaseName(DB_NAME());

    if (!db.open()) {
        qDebug() << "Ошибка подключения к базе данных для поиска таблиц: " << db.lastError().text();

        return false;
    }

    QSqlQuery query;

    query.prepare("SELECT 1 FROM information_schema.tables WHERE table_name = :tablename");
    query.bindValue(":tablename", tableName);

    if (query.exec() && query.next()) {
        qDebug() << "Таблица" << tableName << "найдена.";

        return true;
    }

    qDebug() << "Таблица" << tableName << " не найдена.";


    return false;
}

void DBManager::CreateTable(const QString &tableName)
{
    // подключаемся к БД
    db.setDatabaseName(DB_NAME());

    if (!db.open()) {
        qDebug() << "Ошибка подключения к базе данных для создания таблиц: " << db.lastError().text();

        return;
    }

    QSqlQuery query;
    QString createTableQuery;

    if (tableName == "questions") {
        createTableQuery = QString(
            "CREATE TABLE %1 ("
            "id SERIAL PRIMARY KEY,"
            "question_text TEXT NOT NULL,"
            "answer_text TEXT NOT NULL,"
            "part INT,"
            "source TEXT,"
            "creation_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
            "question_size INTEGER GENERATED ALWAYS AS (char_length(question_text)) STORED,"
            "status VARCHAR(10) CHECK (status IN ('unverified', 'confirmed', 'outdated')) DEFAULT 'unverified')"
        ).arg(tableName);
    }
    else if (tableName == "settings") {
        createTableQuery = QString(
            "CREATE TABLE %1 ("
            "id SERIAL PRIMARY KEY,"
            "option_name VARCHAR(255),"
            "option_value VARCHAR(255),"
            "updated_at TIMESTAMP)"
        ).arg(tableName);
    }
    else if (tableName == "users") {
        createTableQuery = QString(
            "CREATE TABLE %1 ("
            "id SERIAL PRIMARY KEY,"
            "username VARCHAR(50) UNIQUE NOT NULL,"
            "password_hash VARCHAR(255) NOT NULL,"
            "registration_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
            "user_type VARCHAR(5) CHECK (user_type IN ('admin', 'user')) DEFAULT 'user',"
            "status VARCHAR(10) CHECK (status IN ('unverified', 'confirmed', 'outdated')) DEFAULT 'unverified')"
        ).arg(tableName);
    }
    else if (tableName == "logs") {
        createTableQuery = QString(
            "CREATE TABLE %1 ("
            "id SERIAL PRIMARY KEY,"
            "log_message TEXT,"
            "log_level VARCHAR(50),"
            "created_at TIMESTAMP)"
        ).arg(tableName);
    }

    if (query.exec(createTableQuery)) {
        qDebug() << "Таблица" << tableName << "создана.";
    }
    else {
        qDebug() << "Ошибка создания таблицы: " << tableName << query.lastError().text();
    }
}

void DBManager::EnsureTablesExist()
{
    // подключаемся к БД
    db.setDatabaseName(DB_NAME());

    if (!db.open()) {
        qDebug() << "Ошибка подключения к базе данных и поиска таблиц: " << db.lastError().text();

        return;
    }

    // проверяем наличие таблиц и создаем их, если они отсутствуют
    QStringList tableNames = { "questions", "settings", "users", "logs" };

    for (const QString &tableName : tableNames) {
        if (!FindTable(tableName)) {
            CreateTable(tableName);
        }
    }
}

void DBManager::TablePrint(const QString &tableName)
{
    // подключаемся к БД
    db.setDatabaseName(DB_NAME());

    if (!db.open()) {
        qDebug() << "Ошибка подключения к базе данных для печати: " << db.lastError().text();

        return;
    }

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
    }
    else {
        qDebug() << "Ошибка запроса к таблице: " << query.lastError().text();
    }
}

void DBManager::AddRecords(const QString &tableName, int count)
{
    if (!db.isOpen() && !db.open()) {
        qDebug() << "Ошибка подключения к базе данных для добавления записей:" << db.lastError().text();

        return;
    }

    QSqlQuery query;

    for (int i = 0; i < count; ++i) {
        if (tableName == "questions") {
            query.prepare("INSERT INTO questions (question_text, answer_text, part, source, status) VALUES (:question_text, :answer_text, :part, :source, :status)");
            query.bindValue(":question_text", QString("Sample question text %1").arg(i));
            query.bindValue(":answer_text", QString("Sample answer text %1").arg(i));
            query.bindValue(":part", i % 3); // примеры для part
            query.bindValue(":source", QString("source_%1").arg(i));
            query.bindValue(":status", "unverified");
        }
        else if (tableName == "settings") {
            query.prepare("INSERT INTO settings (option_name, option_value, updated_at) VALUES (:option_name, :option_value, CURRENT_TIMESTAMP)");
            query.bindValue("option_name", QString("option_%1").arg(i));
            query.bindValue("option_value", QString("value_%1").arg(i));
        }
        else if (tableName == "users") {
            query.prepare("INSERT INTO users (username, password_hash, user_type, status) VALUES (:username, :password_hash, 'user', 'unverified')");
            query.bindValue(":username", QString("user_%1").arg(i));
            query.bindValue(":password_hash", QString("hash_%1").arg(i));
        }
        else if (tableName == "logs") {
            query.prepare("INSERT INTO logs (log_message, log_level, created_at) VALUES (:log_message, :log_level, CURRENT_TIMESTAMP)");
            query.bindValue(":log_message", QString("log message %1").arg(i));
            query.bindValue(":log_level", "INFO");
        }
        else {
            qDebug() << "Неизвестная таблица:" << tableName;

            return;
        }

        // выполнение и проверка запроса
        if (!query.exec()) {
            qDebug() << "Ошибка при добавлении записи в таблицу" << tableName << ":" << query.lastError().text();
        }
    }

    qDebug() << count << "записей добавлено в таблицу" << tableName;
}
