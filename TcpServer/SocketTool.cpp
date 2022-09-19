#include "SocketTool.h"
#include <QDataStream>
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
      client->write(data );
   }
   else {
      conn->write(data );
   }
}
 // 0: socket 1: server;
void SocketTool::createSocket(QString ip,int port ,int type )
{

    mType = type;
    if(0==type)
    {
         client = new QTcpSocket(this);

         client->connectToHost(QHostAddress(ip),port);

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
         server->listen(QHostAddress::Any,port);

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
                //char tmp[1024]={0};
               // conn->read(tmp,sizeof(tmp));
               // QByteArray array= QByteArray(tmp,sizeof(tmp)) ;// = conn->readAll();
//                QByteArray array =conn->read (1024);
//                QString string;
//                string.prepend(array);
//                emit sigReceiveData(0,string);

                while(conn->bytesAvailable())
                   {
                       qint64 total = 0; // 需要获取的总大小
                       //conn->read((char *)&total, sizeof(int));
                       QByteArray readSizeBuf = conn->read(sizeof(qint64));
                       QDataStream inSizeFile(readSizeBuf);
                       inSizeFile>>total;
                       qDebug() << "total==============" <<QString::number( total);

                       QByteArray readBuf;
                       qint64 readNum = total-sizeof(qint64);
                       //把数据全部读完
                       while(true)
                       {
                           QByteArray tmp = conn->read(readNum);

                           readNum -= tmp.length();
                           readBuf.append(tmp);
                           if(readNum > 0)
                               conn->waitForReadyRead();
                           else
                           {
                               //qDebug() << "recv data from server:" << readBuf;
                               qDebug() << "recv data size from server:" << readBuf.size();
                               QDataStream inFile(readBuf);
                               uchar flag = 0;
                               qint64 filenameSize = 0;
                               QByteArray cbyteArray;
                               QString filename;
                               inFile >>flag>>filenameSize>>filename>>cbyteArray;

      qDebug() << "recv filename from server:" << filename<<" filenameSize "<<QString::number(filenameSize,10);;
                               QString len = QString::number(filenameSize+(sizeof (qint64)*2)+1+cbyteArray.size(),10);
                               qDebug() << "recv len from server:" << len<<" byte len "<<QString::number(cbyteArray.size());

//                               if(total!=filename.length()+sizeof (qint64)*2+1+cbyteArray.size())
//                               {
//                                   qDebug() << "recv data error client===:"  ;
//                               }
//                               else
                               {
                                   ApproachPositionStruct position;

                                   memcpy(&position,cbyteArray.data(),sizeof(ApproachPositionStruct) );
                                    QString string = QString::number(position.Part,10);
                                   // string.prepend(cbyteArray);
                                    qDebug() << "recv data string===:" <<string ;
                                    emit sigReceiveData(0,string);
                               }
                                break;
                           }

                       }
                   }
            });
         });
    }
}
