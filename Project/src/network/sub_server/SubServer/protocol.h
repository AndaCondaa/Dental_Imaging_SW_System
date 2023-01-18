#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QTcpSocket>
#include <QStringList>
#include <QDataStream>


typedef enum {  // 첫 연결 시, type 부분에 들어갈 수 있는 enum 클래스
    SW,
    MODALITY
} ConnectType;

typedef enum {  // 명령 전달 시, type 부분에 들어갈 수 있는 enum 클래스
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
    PacketData *m_packetData;
};

#endif // PROTOCOL_H
