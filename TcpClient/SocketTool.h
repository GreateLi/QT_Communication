#ifndef TCPTOOLS_H
#define TCPTOOLS_H

#include <QObject>
#include<QTcpSocket>
#include<QTcpServer>
#include <QMutex>

#include <QMutexLocker>

#include <QString>
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
    qint64 SendDataArray(QByteArray data);
    QTcpServer* getServer();

private slots:
    void sltServerDisConnect();         //服务端发来断开信号


     void  onConnect();
     void  displayError(QAbstractSocket::SocketError soket_error);
protected:
    SocketTool();
    static SocketTool* m_instance;

    QMutex *m_mutex;
private:
       QTcpSocket *client;
       QTcpServer *server;  //监听套接字
       QTcpSocket *conn;  //通信套接字


       int mType;
};

#endif // TCPTOOLS_H
