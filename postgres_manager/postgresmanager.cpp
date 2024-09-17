#include "postgresmanager.h"

PostgresManager::PostgresManager(const QString& host, const QString& user, const QString& password, int port)
    : m_host(host), m_user(user), m_password(password), m_port(port)
{}

PostgresManager::~PostgresManager() {}

// метод для создания базы данных
bool PostgresManager::createDatabase(const QString& dbName)
{
    QSqlDatabase db = connectToDatabase("postgres");
    if(!db.isOpen()) return false;

    QString query = QString("CREATE DATABASE %1").arg(dbName);
    return executeQuery(db, query);
}

// метод для проверки существования базы данных
bool PostgresManager::databaseExists(const QString& dbName)
{
    // подключаемся к системной базе данных "postgres"
    QSqlDatabase db = connectToDatabase("postgres");
    if(!db.isOpen()) return false;

    // запрос для проверки существования базы данных
    QString query = QString("SELECT 1 FROM pg_database WHERE datname = '%1'").arg(dbName);
    QSqlQuery qry(db);

    // выполняем запрос и проверяем результат
    if(qry.exec(query)) {
        if(qry.next()) return true;
    }
    else {
        qDebug() << "Ошибка при выполнении запроса:" << qry.lastError().text();
    }


    // база данных не найдена
    return false;
}

// метод для уаления базы данных
bool PostgresManager::dropDatabase(const QString& dbName)
{
    // Подключаемся к системной базе данных "postgres"
    QSqlDatabase db = connectToDatabase("postgres");
    if (!db.isOpen()) return false;

    // Принудительно завершаем все сеансы, использующие данную базу данных
    QString terminateConnectionsQuery = QString(
        "SELECT pg_terminate_backend(pg_stat_activity.pid) "
        "FROM pg_stat_activity "
        "WHERE pg_stat_activity.datname = '%1' "
        "AND pid <> pg_backend_pid();"
    ).arg(dbName);

    QSqlQuery qry(db);
    if (!qry.exec(terminateConnectionsQuery)) {
        qDebug() << "Ошибка при завершении активных сеансов:" << qry.lastError().text();
        return false;
    }

    // Запрос на удаление базы данных
    QString dropDbQuery = QString("DROP DATABASE IF EXISTS %1").arg(dbName);
    if (!qry.exec(dropDbQuery)) {
        qDebug() << "Ошибка выполнения запроса:" << qry.lastError().text();
        return false;
    }

    //qDebug() << "База данных успешно удалена!";
    return true;
}

// метод для создания таблицы
bool PostgresManager::createTable(const QString& dbName, const QString& createQuery)
{
    QSqlDatabase db = connectToDatabase(dbName);
    if(!db.isOpen()) return false;

    return executeQuery(db, createQuery);
}

// метод для проверки существования таблицы
bool PostgresManager::tableExists(const QString& dbName, const QString& tableName)
{
    QSqlDatabase db = connectToDatabase(dbName);
    if(!db.isOpen()) return false;

    QString query = QString("SELECT 1 FROM information_schema.tables WHERE table_name = '%1'").arg(tableName);
    QSqlQuery qry(db);
    if(qry.exec(query) && qry.next()) return true;

    return false;
}

// метод для удаления таблицы
bool PostgresManager::dropTable(const QString& dbName, const QString& tableName)
{
    QSqlDatabase db = connectToDatabase(dbName);
    if(!db.isOpen()) return false;

    QString query = QString("DROP TABLE IF EXISTS %1").arg(tableName);
    return executeQuery(db, query);
}

// метод для подключения к базе данных с обработкой ошибок
QSqlDatabase PostgresManager::connectToDatabase(const QString& dbName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");

    db.setHostName(m_host);
    db.setUserName(m_user);
    db.setPassword(m_password);
    db.setPort(m_port);
    db.setDatabaseName(dbName);

    if(!db.open()) {
        QSqlError error = db.lastError();
        if(error.type() == QSqlError::ConnectionError) {
            QString errorMessage = error.text().toLower();
            if(errorMessage.contains("password authentication failed")) {
                qDebug() << "Ошибка аутентификации: неверные логин или пароль.";
            }
            else {
                qDebug() << "Ошибка подключения к базе данных:" << error.text();
            }
        }
        else {
            qDebug() << "Ошибка базы данных: " << error.text();
        }
    }


    return db;
}

// вcпомогательный метод для выполнения SQL-запроса
bool PostgresManager::executeQuery(QSqlDatabase& db, const QString& query)
{
    QSqlQuery qry(db);
    if(!qry.exec(query)) {
        qDebug() << "Ошибка выполнения запроса:" << qry.lastError().text();

        return false;
    }

    return true;
}
