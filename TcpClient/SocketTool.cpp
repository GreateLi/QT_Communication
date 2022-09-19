#include "SocketTool.h"

SocketTool* SocketTool::m_instance = 0;



SocketTool::SocketTool()
{
    this->m_mutex = new QMutex(QMutex::RecursionMode::Recursive);

    //QObject::connect(&timer, &QTimer::timeout, this, &NDISignal::slotCheckNdiStatueTimeout, Qt::UniqueConnection);
}

SocketTool::~SocketTool()
{
    delete this->m_mutex;
}

SocketTool* SocketTool::getInstance()
{
    static QMutex mutex;
    if (!m_instance) {
        QMutexLocker locker(&mutex);
        if (!m_instance) {
            m_instance = new SocketTool();
        }
    }

    return m_instance;
}

void SocketTool::SendMessage(QString msg)
{
    QByteArray data = msg.toUtf8();
   if(0==mType)
   {
      client->write(data,1024 );
   }
   else {
      conn->write(data ,1024);
   }
}

qint64 SocketTool::SendDataArray(QByteArray data)
{
    qint64 slen =0;
    if(0==mType)
    {
      slen =  client->write(data,data.length());

      if(!client->waitForBytesWritten())
       {
           qDebug() << "write error";
       }
      if (!client->flush())
      {
           qDebug()  << "==================socket flush failed  "  ;
      }
    }
    else {
      slen = conn->write(data ,data.length());
    }

    return slen;
}
 // 0: socket 1: server;
void SocketTool::createSocket(QString ip,int port ,int type )
{

    mType = type;
    if(0==type)
    {
         client = new QTcpSocket(this);


         connect(client, SIGNAL(disconnected()), this, SLOT(sltServerDisConnect()));


         connect(client, SIGNAL(error(QAbstractSocket::SocketError)),
                  this, SLOT(displayError(QAbstractSocket::SocketError)));
         connect(client, SIGNAL(connected()), this, SLOT(onConnect()));

        // client->abort();
         client->connectToHost(QHostAddress(ip),port);

         qDebug()<<"conncet to server ip"<<ip<<" port "<<QString::number(port);
         if(!client->waitForConnected(30000))
         {
             qDebug()<<"conncet to server is timeout";

         }
         else
         {
             qDebug()<<"conncet to server is succ";
         }



         //接受数据
         connect(client,&QTcpSocket::readyRead,this,[=](){

            QByteArray array = client->readAll();
            QString string;
            string.prepend(array);
            emit sigReceiveData(0,string);
         });


    }
    else {
         server = new QTcpServer(this);

         //监听
         server->listen(QHostAddress(ip),port);

         //新的连接
         connect(server,&QTcpServer::newConnection,this,[=]()
         {
            //接收客户端的套接字
            conn = server->nextPendingConnection();
            emit sigReceiveData(1);
            //因为客户端每连接一个 就会初始化conn所以放到里面 确保不被重新初始化
            connect(conn,&QTcpSocket::readyRead,this,[=]()
            {
                //接受数据
                QByteArray array = conn->readAll();
                QString string;
                string.prepend(array);
                emit sigReceiveData(0,string);
            });
         });
    }
}

void SocketTool::sltServerDisConnect()
{
    qDebug() << QString::fromLocal8Bit("server close")  ;

}

//void SocketTool::sltDisConnect()
//{
//    if(client)
//    {
//        qDebug() << QString::fromLocal8Bit("disconnect server") << m_nID;

////        m_tcpSocket->disconnectFromHost();
////        m_tcpSocket->deleteLater();
////        m_tcpSocket = NULL;
//    }
//}

void SocketTool::onConnect()
{
    qDebug() << "Connected!";

}

void SocketTool::displayError(QAbstractSocket::SocketError soket_error)
{
    QTcpSocket* sock = (QTcpSocket*)sender();


    qDebug() << "Socket error:" << sock->errorString();
}
