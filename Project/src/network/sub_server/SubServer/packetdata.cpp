/*
 * 프로그램명 : SubServer
 * 파일명 : packetdata.cpp
 * 설명 : 서브서버용 패킷 데이터 클래스
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.12
 */


#include "packetdata.h"

#include <QIODevice>

PacketData::PacketData(QString header, QString event, int type, QString msg, QObject* parent)
    : QObject(parent), m_header(header), m_event(event), m_type(type), m_msg(msg)
{

}

QString PacketData::header()
{
    return m_header;
}

void PacketData::setHeader(QString header)
{
    if (m_header != header) {
        m_header = header;
        emit headerChanged();
    }
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
    out << m_header;
    out << m_event;
    out << m_type;
    out << m_msg;

    return sendArray;
}
