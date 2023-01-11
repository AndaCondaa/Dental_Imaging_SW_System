#include "protocol.h"
#include "packetdata.h"

#include <QByteArray>

Protocol::Protocol()
{
    m_packetData = new PacketData();
}

Protocol::~Protocol()
{
    delete m_packetData;
}

void Protocol::sendProtocol(QTcpSocket* socket, QString event, int pid, QString msg)
{
    m_packetData->setEvent(event);
    m_packetData->setPid(pid);
    m_packetData->setMsg(msg);

    socket->write(m_packetData->makeSendData());
}


