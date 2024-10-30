#include "constants.hpp"
#include "dbmanager.h"
#include <QSqlTableModel>
#include <QApplication>
#include <QMessageBox>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QTableView>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DBManager dbManager;

    // проверка подключения к серверу
    if (!dbManager.ServerAvailable()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не найден сервер PostgreSQL");

        return 0;
    }

    // проверка или создание базы данных
    if (!dbManager.FindDatabase()) {
        dbManager.CreateDatabase();
    }

    // проверка наличия таблиц и создание их, если они отсутствуют
    dbManager.EnsureTablesExist();
    dbManager.AddRecords(TABLE_NAME(0), 200);

    // создаем главное окно приложения
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Рабочая база '" + DB_NAME() + "'");
    mainWindow.resize(1200, 800);

    // создаем виджет для отображения таблицы
    QWidget *centralWidget = new QWidget(&mainWindow);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // создаем компонент QTableView для отображения таблицы
    QTableView *tableView = new QTableView(centralWidget);

    // подключаем таблицу из базы данных к модели
    QSqlTableModel *model = new QSqlTableModel(&mainWindow);
    model->setTable(TABLE_NAME(0)); // "questions"
    model->select(); // загружаем данные

    // настраиваем таблицу для отображения
    tableView->setModel(model);
    tableView->resizeColumnsToContents();

    // добавляем таблицу в основной макет
    layout->addWidget(tableView);
    centralWidget->setLayout(layout);

    // устанавливаем центральный виджет в главное окно
    mainWindow.setCentralWidget(centralWidget);
    mainWindow.show();


    return app.exec();
}
