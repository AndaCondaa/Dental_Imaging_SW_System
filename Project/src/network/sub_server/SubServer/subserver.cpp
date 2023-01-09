/*
 * 프로그램명 : SubServer
 * 파일명 : subserver.cpp
 * 설명 : 영상 검사실용 미니 서버 (촬영SW와 Modality의 연결)
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.01.06
 */

#include "subserver.h"
#include "ui_subserver.h"

#include "protocol.h"

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
        // 서버 listen 실패
    } else {
        //ui->logEdit->append("SERVER OPEN!");
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
    connect(sockConnection, SIGNAL(readyRead()), this, SLOT(receiveSocketFromClient()));
}

void SubServer::sendSocketToClient(QTcpSocket* sock, QString header, QString event,QString pid)
{
    QByteArray dataArray;
    QDataStream out(&dataArray, QIODevice::WriteOnly);
    out.device()->seek(0);
    sock->write(dataArray);
    sock->flush();
    while(sock->waitForBytesWritten());
}

void SubServer::receiveSocketFromClient()
{
    QString receiveMsg;
    Type type;

    QTcpSocket *receiveSocket = qobject_cast<QTcpSocket*>(sender());

    QByteArray byteArray = receiveSocket->readAll();
    QDataStream in(&byteArray, QIODevice::ReadOnly);
    in.device()->seek(0);
    in >> type;
    in >> receiveMsg;

    if (type == Data) {
        ui->logEdit->append(receiveMsg);
    }

}

