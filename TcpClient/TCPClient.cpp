#include "TCPClient.h"
#include "ui_client.h"
#include<QTcpSocket>
#include<QHostAddress>
#include <QThreadPool>
#include <QtConcurrent/QtConcurrent>
#include <QThread>
#include "SocketTool.h"
#include "test.h"

inline void printTID(const QString& str="")
{
    qDebug() << QThread::currentThreadId() << str;
}

TCPClient::TCPClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{

    ui->setupUi(this);
    ui->Sip->setText("192.168.124.11");
    ui->Sport->setText("10002");

//    QString ip = ui->Sip->text();

//     QString port = ui->Sport->text();
//    Test* test = nullptr;
//    QThreadPool pool;
//    printTID(":tmain");
//    QtConcurrent::run(&pool, [&test,ip,port](){
//        test = new Test;
//        printTID(":t1");
//        SocketTool::getInstance()->createSocket(ip,port.toInt(),0);

////        connect(SocketTool::getInstance(), SIGNAL(sigReceiveData(int, QString)),
////                this, SLOT(sltReceive(int, QString)));
//        QEventLoop e;
//        QObject::connect(test, &Test::test1, test/*lambda表达式执行在创建该对象的线程中*/,[](const QString& str){
//            printTID(":我在t1中执行 " + str);
//        });
//        QObject::connect(test, &Test::test2/*lambda表达式执行在创建信号发射对象的线程中*/, [](const QString& str){
//            printTID(":我在t2或t3中执行" + str);
//        });
//        e.exec();
//    });


   SocketTool::getInstance()->createSocket(ui->Sip->text(),ui->Sport->text().toInt(),0);
            connect(SocketTool::getInstance(), SIGNAL(sigReceiveData(int, QString)),
                    this, SLOT(sltReceive(int, QString)));
    mQTimer.setInterval(10);
    //connect(&m_AutoConnectedTimer, SIGNAL(timeout()), this, SLOT(slotAutoConnectedTimer()));
    connect(&mQTimer,SIGNAL(timeout()),this,SLOT(slotSendMessage()),Qt::DirectConnection);

    connect(this,SIGNAL(signalAppandText(QString)),this,SLOT(slotAppandText(QString)),Qt::QueuedConnection);
    mStartClock = std::chrono::steady_clock::now();
}

void  TCPClient:: sltReceive(int type, QString msg)
{
    if(0==type)
    {
        ui->recorb->append(msg);
    }
    else {
        ui->recorb->append("new client...");
    }
}

void TCPClient::slotSendMessage()
{
    ApproachPositionStruct approach;

    approach.Part =  (std::chrono::steady_clock::now() - mStartClock).count();
    QString str= QString::number(approach.Part,10);
    outBlock.resize(0);
    QByteArray barray ;
    barray.append((char*)&approach, sizeof(ApproachPositionStruct));
    QString type = "slotSendMessage";
    int dataSize =  barray.size();
   // this->totalBytes = dataSize;
    QDataStream sendout(&outBlock,QIODevice::WriteOnly);

    qDebug()<<"robot Control type"<< type<<"   len" <<QString::number(type.length());
    qDebug()<<"robot dataSize"<<QString::number(dataSize,10) ;

    sendout<<qint64(0)<<uchar(0)<<qint64(0)<<type ;
    //totalBytes = outBlock.size();
    sendout.device()->seek(0);

    qDebug()<<"filenamesize  "<< QString::number( outBlock.size()-sizeof(qint64)*2-1);
    if(dataSize>0)
    {
       sendout<<totalBytes<<uchar(0)<<qint64(outBlock.size()-(sizeof(qint64)*2)-1)<<type<<barray;
    }
    else {
       sendout<<totalBytes<<uchar(0)<<qint64(outBlock.size()-(sizeof(qint64)*2)-1)<<type;
    }

    totalBytes = outBlock.size();
    sendout.device()->seek(0);
    sendout<<totalBytes;
    sendout.device()->seek(totalBytes-1);
    qDebug()<<"totalBytes:" <<QString::number(totalBytes,10) ;

    qDebug()<<"outBlock:" <<QString::number(outBlock.size(),10) ;
    int bytestoWrite =  SocketTool::getInstance()->SendDataArray(outBlock );
    qDebug()<<"bytestoWrite:" <<QString::number(bytestoWrite,10)    ;
    outBlock.resize(0);
       qDebug()<<"str:" <<str ;
    emit signalAppandText(str);
}

TCPClient::~TCPClient()
{
    delete ui;
}

void TCPClient::on_send_clicked()
{
//    SocketTool::getInstance()->SendMessage(ui->msg->toPlainText());
//    ui->recorb->append("Client__>>:"+ui->msg->toPlainText().toUtf8());
//    ui->msg->clear();
    if(!mQTimer.isActive())
    {
        mQTimer.start();
    }
}

void TCPClient::slotAppandText(QString txt)
{
    ui->recorb->append(txt);
}

void TCPClient::on_pushbtnStop_clicked()
{
     mQTimer.stop();
}
