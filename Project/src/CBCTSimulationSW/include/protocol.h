#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QTcpSocket>
#include <QStringList>
#include <QDataStream>


typedef enum {
    SW,
    MODALITY
} ConnectType;

typedef enum {
    RESET,
    READY,
    START,
    STOP
} ControlType;


class PacketData;

class Protocol
{
public:
    Protocol();
    ~Protocol();

    PacketData* packetData();

    // send
    void sendProtocol(QTcpSocket* socket, QString event, int type, QString msg);

    // receive
    void receiveProtocol(QTcpSocket* socket);

private:
    PacketData* m_packetData;
};

#endif // PROTOCOL_H