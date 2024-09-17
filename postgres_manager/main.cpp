#include "postgresmanager.h"
#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    // подключение к postgres
    PostgresManager manager("localhost", "postgres", "postgres", 5432);

    // создание базы данных
    if(manager.createDatabase("storehouse")) {
        qDebug() << "База данных успешно создана!";
    }
    else {
        qDebug() << "Ошибка при создании базы данных.";
    }

    // поиск базы данных
    if(manager.databaseExists("storehouse")) {
        qDebug() << "База данных найдена.";
    }
    else {
        qDebug() << "База данных не найдена.";
    }

    // удаление базы данных
    if(manager.dropDatabase("storehouse")) {
        qDebug() << "База данных успешно удалена!";
    } else {
        qDebug() << "Ошибка при удалении базы данных.";
    }

    w.show();


    return a.exec();
}
