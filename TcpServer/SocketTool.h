#ifndef TCPTOOLS_H
#define TCPTOOLS_H

#include <QObject>
#include<QTcpSocket>
#include<QTcpServer>
#include <QMutex>

#include <QMutexLocker>

#include <QString>

typedef struct _ApproachPositionStruct {
    double inPoint[3];
    double outPoint[3];

    double leftPoint[3];
    double rightPoint[3];
    qint64 Part;
}ApproachPositionStruct;
class SocketTool : public QObject
{
    Q_OBJECT
public:

    virtual ~SocketTool();
    static SocketTool* getInstance();
signals:


    void sigReceiveData(int type=0, QString socketInfo=""); //
public:
    void createSocket(QString ip,int port ,int type);// 0: socket 1: server;
    void SendMessage(QString msg);
    QTcpServer* getServer();
protected:
    SocketTool();
    static SocketTool* m_instance;

    QMutex *m_mutex;
private:
       QTcpSocket *client;
       QTcpServer *server;
       QTcpSocket *conn;
       int mType;
};

#endif // TCPTOOLS_H
