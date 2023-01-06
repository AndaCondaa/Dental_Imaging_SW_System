/*
 * 프로그램명 : ImagingSW
 * 파일명 : controlsocket.cpp
 * 설명 : 영상장비에 대한 제어명령 전송 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.01.06
 */

#include "controlsocket.h"

#include <QTcpSocket>

#include "protocol.h"

ControlSocket::ControlSocket(QObject *parent)
    : QObject{parent}
{
    socket = new QTcpSocket(this);

    connectToSubServer();
}

ControlSocket::~ControlSocket()
{
    socket->close();
    delete socket;
}

void ControlSocket::connectToSubServer()
{
    socket->connectToHost("192.168.0.46", 8000);
    if (socket->waitForConnected()) {
        qDebug("ControlSocket Connetion Success!");
        connect(socket, SIGNAL(readyRead()), SLOT(receiveSocketFromSubServer()));

        sendSocketToSubServer(socket, "NEW", "CONNECT", "ISW");
    } else {
        // 연결 실패 예외처리
    }
}

void ControlSocket::sendSocketToSubServer(QTcpSocket* sock, QString header, QString event, QString pid)
{
    //QString sendMsg = header + "^" + event + "<CR>" + pid + "<END>";
    QStringList sendMsg;
    sendMsg << header;
    sendMsg << event;
    sendMsg << pid;
    int size = sizeof(sendMsg);

    QByteArray dataArray;
    QDataStream out(&dataArray, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << size;
    out << sendMsg;
    socket->write(dataArray);
    socket->flush();
    while(socket->waitForBytesWritten());
}

void ControlSocket::receiveSocketFromSubServer()
{
}




