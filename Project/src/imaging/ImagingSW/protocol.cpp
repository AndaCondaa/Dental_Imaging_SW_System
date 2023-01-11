#include "protocol.h"
#include "packetdata.h"

#include <QDataStream>

Protocol::Protocol(QTcpSocket *socket) : m_socket(socket)
{
    packetData = new PacketData();
}

Protocol::~Protocol()
{
    m_socket->close();
    delete m_socket;
    delete packetData;
}

QTcpSocket* Protocol::socket()
{
    return m_socket;
}

void Protocol::setSocket(QTcpSocket *socket)
{
    m_socket = socket;
}

void Protocol::sendProtocol(QString event, int pid, QString msg)
{
    packetData->setEvent(event);
    packetData->setPid(pid);
    packetData->setMsg(msg);

    QDataStream *sendData = new QDataStream;
    sendData = packetData->data();
}


