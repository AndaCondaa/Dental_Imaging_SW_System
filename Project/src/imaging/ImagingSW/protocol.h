#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_PKT 1024

#include <QTcpSocket>
#include <QStringList>
#include <QDataStream>

typedef enum { Data, Ack } Type;

typedef struct {
    Type type;
    char msg[MAX_PKT];
} Packet;

class Protocol
{
public:
    Protocol();
    Protocol(QTcpSocket*);
    ~Protocol();

    void setSocket(QTcpSocket *socket);
    QTcpSocket* getSocket();

    void sendProtocolToServer(QTcpSocket *socket, Type type, QString header, QString event, QString msg, QString PID);
    QString makeProtocolData(QStringList dataList);

    QTcpSocket *memberSocket;
    Packet *packet;
};







#endif // PROTOCOL_H
