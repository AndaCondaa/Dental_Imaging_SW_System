#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_PKT 1024

class QTcpSocket;
class QString;
class QStringList;

typedef enum { Data, Ack } Type;

typedef struct {
    Type type;
    char msg[MAX_PKT];
} Packet;


void sendProtocolToServer(QTcpSocket *socket, Type type, QString header, QString event, QString msg, QString PID);
QString makeProtocolData(QStringList dataFromClient);




#endif // PROTOCOL_H
