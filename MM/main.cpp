#include <QApplication>
#include <QVBoxLayout>
#include <QTextStream>
#include <QMessageBox>
#include <QWidget>
#include <QLabel>
#include <QFile>


class ConfigReaderWidget : public QWidget {
public:
    ConfigReaderWidget(QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout *layout = new QVBoxLayout(this);
        QLabel *label = new QLabel(this);

        // Чтение содержимого файла conf.cfg
        QFile file("E:/git/MM/MM/build/Desktop_Qt_6_7_3_MinGW_64_bit-Debug/debug/conf.cfg");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            QString content = in.readAll();
            label->setText(content);  // Вывод содержимого в метку
            file.close();
        } else {
            // Сообщение об ошибке, если файл не может быть открыт
            QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл conf.cfg");
        }

        layout->addWidget(label);
        setLayout(layout);
    }
};


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ConfigReaderWidget widget;
    widget.setWindowTitle("Содержимое файла conf.cfg");
    widget.resize(400, 300);
    widget.show();

    return app.exec();
}
