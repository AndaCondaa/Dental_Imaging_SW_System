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
    if(!server->listen(QHostAddress::Any, 8000)) {
        ui->logEdit->append("SERVER OPEN!");
    }
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

    QByteArray byteArray = receiveSocket->read(1024);
    Protocol_Type type;
    char data[1020];
    memset(data, 0, 1020);

    QDataStream in(&byteArray, QIODevice::ReadOnly);
    in.device()->seek(0);
    in >> type;
    in.readRawData(data, 1020);

    switch (type) {
    case Test1:
        ui->logEdit->append(data);
        break;
    }

}

void SubServer::sendProtocol(QTcpSocket* sock, Protocol_Type type, QString data, int size)
{
    QByteArray dataArray;
    QDataStream out(&dataArray, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << type;
    out.writeRawData(data.toStdString().data(), size);
    sock->write(dataArray);
    sock->flush();
    while(sock->waitForBytesWritten());
}
