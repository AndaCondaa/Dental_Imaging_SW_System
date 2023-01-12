#include "packetdata.h"

#include <QIODevice>

PacketData::PacketData(QString event, int index, QString msg, QObject* parent)
    : QObject(parent), m_event(event), m_index(index), m_msg(msg)
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

int PacketData::index()
{
    return m_index;
}

void PacketData::setIndex(int index)
{
    if(m_index != index) {
        m_index = index;
        emit indexChanged();
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
    out << m_index;
    out << m_msg;

    return sendArray;
}
