#include"TCPClient.h"
#include <QApplication>
#include <QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TCPClient client;
    QFile file(":/Ubuntu.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet =  (file.readAll());
    client.setStyleSheet(styleSheet);
    client.setWindowTitle("TCPClient");
    client.show();

    return a.exec();
}
