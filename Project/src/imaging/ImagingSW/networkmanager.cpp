/*
 * 프로그램명 : ImagingSW
 * 파일명 : controlsocket.cpp
 * 설명 : 영상장비에 대한 제어명령 전송 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.01.06
 */

#include "networkmanager.h"
#include "protocol.h"

NetworkManager::NetworkManager(QObject *parent)
    : QObject{parent}
{
    controlSocket = new QTcpSocket(this);
    protocol = new Protocol;

    connectToSubServer();
}

NetworkManager::~NetworkManager()
{
    controlSocket->close();
    delete controlSocket;
}

void NetworkManager::connectToSubServer()
{
    controlSocket->connectToHost("127.0.0.1", 8000);
    if (controlSocket->waitForConnected()) {
        connect(controlSocket, SIGNAL(readyRead()), SLOT(receiveSocketFromSubServer()));

        protocol->sendProtocolToServer(controlSocket, Data, "header", "event", "msg", "PID");
    } else {
        // 연결 실패 예외처리
    }
}

void NetworkManager::receiveSocketFromSubServer()
{

}




