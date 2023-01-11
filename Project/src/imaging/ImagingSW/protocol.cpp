#include "protocol.h"

Protocol::Protocol(QTcpSocket *socket)
{
    memberSocket = new QTcpSocket();
    memberSocket = socket;
}

Protocol::~Protocol()
{
    memberSocket->close();
}

void Protocol::setSocket(QTcpSocket *socket)
{
    memberSocket = socket;
}

QTcpSocket* Protocol::getSocket()
{
    return memberSocket;
}

void Protocol::sendProtocol(QString header, QString event, QString PID, QString msg)
{

}

void Protocol::parseSocket(QTcpSocket* socket)
{

}

char* Protocol::makeSendData()
{

    return
}
