#include "TCPserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TCPserver w;
    w.setWindowTitle("Server");
    w.show();

//    Client c;
//    c.setWindowTitle("Client");
//    c.show();

    return a.exec();
}
