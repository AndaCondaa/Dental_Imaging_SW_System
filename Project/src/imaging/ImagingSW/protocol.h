/*
 * 프로그램명 : ImagingSW
 * 파일명 : protocol.h
 * 설명 : 서브서버용 네트워크(프로토콜) API
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.12
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H


class QTcpSocket;
class PacketData;
class QString;

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

class Protocol
{
public:
    Protocol();
    ~Protocol();

    PacketData* packetData();

    // send
    void sendProtocol(QTcpSocket* socket, QString header, QString event, int type, QString msg);

    // receive
    void receiveProtocol(QTcpSocket* socket);

private:
    PacketData *m_packetData;
};

#endif // PROTOCOL_H
