#include "packetdata.h"

PacketData::PacketData(QString event, int pid, QString msg, QObject* parent)
    : QObject(parent), m_event(event), m_pid(pid), m_msg(msg)
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

int PacketData::pid()
{
    return m_pid;
}

void PacketData::setPid(int pid)
{
    if(m_pid != pid) {
        m_pid = pid;
        emit pidChanged();
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

QDataStream& PacketData::data()
{
    QDataStream *stream = new QDataStream;
    *stream << m_event << m_pid << m_msg;
    return *stream;
}
