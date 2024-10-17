#include <QApplication>
#include <QVBoxLayout>
#include <QTextStream>
#include <QMessageBox>
#include <QWidget>
#include <QLabel>
#include <QFile>


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


    return app.exec();
}
