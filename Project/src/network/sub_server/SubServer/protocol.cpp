#include "protocol.h"
#include "packetdata.h"

Protocol::Protocol()
{
    m_packetData = new PacketData();
}

Protocol::~Protocol()
{
    delete m_packetData;
}

PacketData* Protocol::packetData()
{
    return m_packetData;
}

void Protocol::sendProtocol(QTcpSocket* socket, QString event, int type, QString msg)
{
    m_packetData->setEvent(event);
    m_packetData->setType(type);
    m_packetData->setMsg(msg);

    socket->write(m_packetData->makeSendData());
}

void Protocol::receiveProtocol(QTcpSocket* socket)
{
    QString event;
    int type;
    QString msg;

    QByteArray receiveArray = socket->readAll();
    QDataStream in(&receiveArray, QIODevice::ReadOnly);
    in.device()->seek(0);
    in >> event;
    in >> type;
    in >> msg;

    m_packetData->setEvent(event);
    m_packetData->setType(type);
    m_packetData->setMsg(msg);
}
