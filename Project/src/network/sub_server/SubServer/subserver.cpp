#include "subserver.h"
#include "ui_subserver.h"

#include <QTcpServer>
#include <QTcpSocket>

SubServer::SubServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubServer)
{
    ui->setupUi(this);
    sock = nullptr;

    //OpenServer
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newClient()));
}

SubServer::~SubServer()
{
    delete ui;
    server->close();
}

void SubServer::newClient()
{
    QTcpSocket *sockConnection = server->nextPendingConnection();
    connect(sockConnection, SIGNAL(readyRead()), this, SLOT(receiveData()));
    ui->logEdit->append("New Connection!");
}

void SubServer::receiveData()
{
    QTcpSocket *receiveSocket = qobject_cast<QTcpSocket*>(sender());

    QByteArray bytearray = receiveSocket->read(1024);

}
