/*
 * 프로그램명 : ImagingSW
 * 파일명 : networkmanager.cpp
 * 설명 : 영상장비에 대한 제어명령 전송 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.01.09
 */

#include "networkmanager.h"
#include "protocol.h"

NetworkManager::NetworkManager(QObject *parent)
    : QObject{parent}
{
    subSocket = new QTcpSocket(this);
    protocol = new Protocol();

    connectToSubServer("127.0.0.1", 8000);
}

NetworkManager::~NetworkManager()
{
    subSocket->close();
    delete subSocket;
    delete protocol;
}

void NetworkManager::connectToSubServer(QString address, int port)
{
    subSocket->connectToHost(address, port);
    if (subSocket->waitForConnected()) {
        connect(subSocket, SIGNAL(readyRead()), SLOT(receiveSocketFromSubServer()));
        protocol->sendProtocol(subSocket, "CNT", 12345, "MESSAGE:CONNECT");
    } else {
        // 연결 실패 예외처리 구현
    }
}

void NetworkManager::receiveSocketFromSubServer()
{
    subSocket = qobject_cast<QTcpSocket*>(sender());

    QString event;
    int pid;
    QString msg;

    QByteArray receiveArray = subSocket->readAll();
    QDataStream in(&receiveArray, QIODevice::ReadOnly);
    in.device()->seek(0);
    in >> event;
    in >> pid;
    in >> msg;

    qDebug() << event;
    qDebug() << pid;
    qDebug() << msg;
}





