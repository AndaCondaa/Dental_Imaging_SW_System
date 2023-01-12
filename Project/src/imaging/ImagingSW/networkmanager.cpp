/*
 * 프로그램명 : ImagingSW
 * 파일명 : networkmanager.cpp
 * 설명 : 영상장비에 대한 제어명령 전송 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.01.09
 */

#include "networkmanager.h"
#include "protocol.h"
#include "packetdata.h"

#include <windows.h>


NetworkManager::NetworkManager(QObject *parent)
    : QObject{parent}
{
    subSocket = new QTcpSocket(this);
    fileSocket = new QTcpSocket(this);
    protocol = new Protocol();

    connection("127.0.0.1", 8000);
}

NetworkManager::~NetworkManager()
{
    subSocket->close();
    delete subSocket;
    delete protocol;
}

void NetworkManager::connection(QString address, int port)
{
    subSocket->connectToHost(address, port);
    if (subSocket->waitForConnected()) {
        connect(subSocket, SIGNAL(readyRead()), SLOT(receiveSocket()), Qt::DirectConnection);
        protocol->sendProtocol(subSocket, "NEW", SW, "CONTROL");
    } else {
        // 연결 실패 예외처리 구현
    }

    fileSocket->connectToHost(address, port+1);
    if (fileSocket->waitForConnected()) {
        connect(fileSocket, SIGNAL(readyRead()), SLOT(receiveFile()), Qt::DirectConnection);
        protocol->sendProtocol(fileSocket, "NEW", SW, "FILE");
    }
}

void NetworkManager::receiveSocket()
{
    subSocket = dynamic_cast<QTcpSocket*>(sender());
    protocol->receiveProtocol(subSocket);

    qDebug() << protocol->packetData()->event();
    qDebug() << protocol->packetData()->pid();
    qDebug() << protocol->packetData()->msg();
}

void NetworkManager::receiveButtonSignal(int buttonIdx)
{
    protocol->sendProtocol(subSocket, "CNT", buttonIdx, "Pano or Ceph");
}

void NetworkManager::receiveFile()
{

}



