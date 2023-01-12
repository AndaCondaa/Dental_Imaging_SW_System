#include "packetdata.h"

#include <QIODevice>

PacketData::PacketData(QString event, int type, QString msg, QObject* parent)
    : QObject(parent), m_event(event), m_type(type), m_msg(msg)
{

}

QString PacketData::event()
{
    return m_event;
}

void PacketData::setEvent(QString event)
{
    if (m_event != event) {
        m_event = event;
        emit eventChanged();
    }
}

int PacketData::type()
{
    return m_type;
}

void PacketData::setType(int type)
{
    if(m_type != type) {
        m_type = type;
        emit typeChanged();
    }
}

QString PacketData::msg()
{
    return m_msg;
}

void PacketData::setMsg(QString msg)
{
    if(m_msg != msg) {
        m_msg = msg;
        emit msgChanged();
    }
}

QByteArray PacketData::makeSendData()
{
    QByteArray sendArray;
    QDataStream out(&sendArray, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << m_event;
    out << m_type;
    out << m_msg;

    return sendArray;
}
