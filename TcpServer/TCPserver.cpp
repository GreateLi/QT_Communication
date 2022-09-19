#include "TCPserver.h"
#include "ui_server.h"
#include "SocketTool.h"
TCPserver::TCPserver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);

    ui->SIp->setText("127.0.0.1");
    ui->SPort->setText("10002");

    SocketTool::getInstance()->createSocket(ui->SIp->text(),ui->SPort->text().toInt(),
            1 );

    connect(SocketTool::getInstance(), SIGNAL(sigReceiveData(int, QString)),
            this, SLOT(SlotReceive(int, QString)),Qt::QueuedConnection);

}

void TCPserver::SlotReceive(int type, QString socketInfo )
{
    if(0==type)
    {
        ui->recorb->append(socketInfo);
    }
    else {
        ui->recorb->append("a new client...");
    }

}

TCPserver::~TCPserver()
{
    delete ui;
}

void TCPserver::on_send_clicked()
{
    SocketTool::getInstance()->SendMessage(ui->msg->toPlainText());
    ui->recorb->append("Server__>>:"+ ui->msg->toPlainText().toUtf8());
    ui->msg->clear();
}
