#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include<QTcpSocket>
#include<QTimer>

#include <ctime>
#include <ratio>
#include <chrono>

namespace Ui {
class Client;
}

typedef struct _ApproachPositionStruct {
        double inPoint[3];
        double outPoint[3];

        double leftPoint[3];
        double rightPoint[3];
        qint64 Part;
}ApproachPositionStruct;

class TCPClient : public QWidget
{
    Q_OBJECT

public:
    explicit TCPClient(QWidget *parent = 0);
    ~TCPClient();

private:
    Ui::Client *ui;

signals:
    void signalAppandText(QString txt);
public slots:
    void  sltReceive(int type, QString msg);
    void slotSendMessage();
private slots:
    void on_send_clicked();

     void slotAppandText(QString txt);


     void on_pushbtnStop_clicked();

private:
    QTimer mQTimer;

    ///total
    qint64 totalBytes;      //

    qint64 filenameSize;    //

    QString filename;

    QByteArray outBlock;

    std::chrono::steady_clock::time_point mStartClock;
};

#endif // CLIENT_H
