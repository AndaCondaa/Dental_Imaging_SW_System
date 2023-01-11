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
    controlSocket = new QTcpSocket(this);
    protocol = new Protocol(controlSocket);

    connectToSubServer("127.0.0.1", 8000);
}

NetworkManager::~NetworkManager()
{
    controlSocket->close();
    delete controlSocket;
    delete protocol;
}

void NetworkManager::connectToSubServer(QString address, int port)
{
    controlSocket->connectToHost(address, port);
    if (controlSocket->waitForConnected()) {
        connect(controlSocket, SIGNAL(readyRead()), SLOT(receiveSocketFromSubServer()));
        protocol->sendProtocol("event", 123, "message");
    } else {
        // 연결 실패 예외처리 구현
    }
}

void NetworkManager::receiveSocketFromSubServer()
{
    controlSocket = qobject_cast<QTcpSocket*>(sender());

}





