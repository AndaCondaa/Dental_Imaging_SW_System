#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_PKT 1024

class QTcpSocket;

typedef enum { data, ack } Type;

typedef struct {
    Type type;
    char msg[MAX_PKT];
} Packet;


void checkType(QTcpSocket* socket);





#endif // PROTOCOL_H
