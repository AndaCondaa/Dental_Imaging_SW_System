// 프로토콜
// HEADER(3글자)<CR>EVENT(3글자)<CR>PID(5글자예상)<CR>MESSAGE(없을 경우, "NULL")<END>

#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_MSG 1000

#include <QTcpSocket>
#include <QStringList>
#include <QDataStream>

typedef struct {
    QString header;
    QString event;
    QString PID;
    QString msg;
} Packet;

class Protocol
{
public:
    Protocol(QTcpSocket* parentSocket);
    ~Protocol();

    void setSocket(QTcpSocket *socket);
    QTcpSocket* getSocket();

    QStringList makeSendData(QString header, QString event, QString PID, QString msg);
    void sendProtocolToServer(QStringList data);
    void sendProtocolToClient(QTcpSocket *socket, QStringList data);
    QStringList parsingPacket(QTcpSocket* socket);


    QTcpSocket *memberSocket;
    Packet *packet;
};







#endif // PROTOCOL_H
