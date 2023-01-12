#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QTcpSocket>
#include <QStringList>
#include <QDataStream>

#define SW 0
#define MODALITY 1

class PacketData;

class Protocol
{
public:
    Protocol();
    ~Protocol();

    PacketData* packetData();

    // send
    void sendProtocol(QTcpSocket* socket, QString event, int pid, QString msg);

    // receive
    void receiveProtocol(QTcpSocket* socket);

private:
    PacketData *m_packetData;
};

#endif // PROTOCOL_H
