#include <QApplication>
#include <QMessageBox>
#include <QMainWindow>
#include <QTableView>
#include <QVBoxLayout>
#include <QSqlTableModel>
#include "dbmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DBManager dbManager;

    // Проверка подключения к серверу
    if (!dbManager.isServerAvailable()) {
        QMessageBox::critical(nullptr, "Ошибка", "Не найден сервер PostgreSQL");
        return 0;
    }

    // Проверка или создание базы данных
    if (!dbManager.FindDatabase()) {
        dbManager.CreateDatabase();
    }

    // Проверка или создание таблицы
    if (!dbManager.FindTable()) {
        dbManager.CreateTable();
    }

    // Создаем главное окно приложения
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Рабочая база 'mm'");
    mainWindow.resize(800, 600);

    // Создаем виджет для отображения таблицы
    QWidget *centralWidget = new QWidget(&mainWindow);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Создаем компонент QTableView для отображения таблицы
    QTableView *tableView = new QTableView(centralWidget);

    // Подключаем таблицу из базы данных к модели
    QSqlTableModel *model = new QSqlTableModel(&mainWindow);
    model->setTable("settings");
    model->select(); // Загружаем данные

    // Настраиваем таблицу для отображения
    tableView->setModel(model);
    tableView->resizeColumnsToContents();

    // Добавляем таблицу в основной макет
    layout->addWidget(tableView);
    centralWidget->setLayout(layout);

    // Устанавливаем центральный виджет в главное окно
    mainWindow.setCentralWidget(centralWidget);
    mainWindow.show();

    return app.exec();
}
