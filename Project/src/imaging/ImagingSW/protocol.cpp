/*
 * 프로그램명 : ImagingSW
 * 파일명 : protocol.cpp
 * 설명 : 서브서버용 네트워크(프로토콜) API
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.12
 */

#include "protocol.h"
#include "packetdata.h"

#include <QTcpSocket>
#include <QDataStream>

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

void Protocol::sendProtocol(QTcpSocket* socket, QString header, QString event, int type, QString msg)
{
    m_packetData->setHeader(header);
    m_packetData->setEvent(event);
    m_packetData->setType(type);
    m_packetData->setMsg(msg);

    socket->write(m_packetData->makeSendData());
}

void Protocol::receiveProtocol(QTcpSocket* socket)
{
    QString header;
    QString event;
    int type;
    QString msg;

    QByteArray receiveArray = socket->readAll();
    QDataStream in(&receiveArray, QIODevice::ReadOnly);
    in.device()->seek(0);
    in >> header;
    in >> event;
    in >> type;
    in >> msg;

    m_packetData->setHeader(header);
    m_packetData->setEvent(event);
    m_packetData->setType(type);
    m_packetData->setMsg(msg);
}
