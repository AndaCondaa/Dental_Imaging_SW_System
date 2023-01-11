// 프로토콜
// HEADER(3글자)<CR>EVENT(3글자)<CR>PID(5글자예상)<CR>MESSAGE(없을 경우, "NULL")<END>

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QTcpSocket>
#include <QStringList>
#include <QDataStream>

typedef struct {
    QString header;
    QString event;
    int pid;
    QString msg;
} Packet;

class Protocol
{
public:
    Protocol(QTcpSocket* parentSocket);
    ~Protocol();

    void setSocket(QTcpSocket *socket);
    void setData(QStringList data);
    void setMemberPacket();
    QTcpSocket* getSocket();
    QStringList getData();

    // send
    void sendProtocol(QString header, QString event, QString PID, QString msg);

    // receive
    void parseSocket(QTcpSocket* socket);
    void packSocket();

    QTcpSocket *memberSocket;
    Packet *packet;
    QStringList data;
};







#endif // PROTOCOL_H
