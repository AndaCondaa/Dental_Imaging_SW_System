#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QTcpSocket>
#include <QStringList>
#include <QDataStream>

class PacketData;

class Protocol
{
public:
    Protocol();
    ~Protocol();

    // send
    void sendProtocol(QTcpSocket* soket, QString event, int pid, QString msg);


private:
    PacketData *m_packetData;

signals:
    void socketChanged();
};

#endif // PROTOCOL_H
