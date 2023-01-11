#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QTcpSocket>
#include <QStringList>
#include <QDataStream>

class PacketData;

class Protocol
{
    Q_PROPERTY(QTcpSocket* socket READ socket WRITE setSocket NOTIFY socketChanged)

public:
    explicit Protocol(QTcpSocket* parentSocket);
    ~Protocol();


    QTcpSocket* socket();
    void setSocket(QTcpSocket *socket);


    // send
    void sendProtocol(QString event, int pid, QString msg);


private:
    QTcpSocket *m_socket;
    PacketData *packetData;

signals:
    void socketChanged();

};







#endif // PROTOCOL_H
