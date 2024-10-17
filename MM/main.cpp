#include <QApplication>
#include <QSqlDatabase>
#include <QVBoxLayout>
//#include <QTextStream>
//#include <QMessageBox>
#include <QSqlError>
#include <QWidget>
//#include <QLabel>
//#include <QFile>


class DBManager : public QWidget{
    Q_OBJECT

public:
    DBManager(QWidget *parent = nullptr) : QWidget(parent)
    {

    }
};


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);


    QWidget w;
    w.setWindowTitle("MM");
    w.resize(1000, 700);
    w.show();


    // проверка psql
    // параметры подключения
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setPort(5432);
    // подключаемся к системной базе
    db.setDatabaseName("postgres");
    db.setUserName("MM_user");
    db.setPassword("U4Xmg3");

    // проверяем подключение
    if (!db.open()) {
        qDebug() << "Ошибка подключения: " << db.lastError().text();
    }


    return app.exec();
}
