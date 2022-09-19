#ifndef SERVER_H
#define SERVER_H

#include <QWidget>

namespace Ui {
class Server;
}


class TCPserver : public QWidget
{
    Q_OBJECT

public:
    explicit TCPserver(QWidget *parent = 0);
    ~TCPserver();
public slots:
    void  SlotReceive(int type=0, QString socketInfo="");
private slots:
    void on_send_clicked();

private:
    Ui::Server *ui;

};

#endif // SERVER_H
