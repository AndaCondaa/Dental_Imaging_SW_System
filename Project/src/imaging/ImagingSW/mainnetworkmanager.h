#ifndef MAINNETWORKMANAGER_H
#define MAINNETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>

class MainNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit MainNetworkManager(QObject *parent = nullptr);
    ~MainNetworkManager();

private slots:
    void connection(QString address, int port);
    void sendPacket(QTcpSocket* socket, QString event, QString pid, QString data);
    QStringList packetParser(QByteArray receiveArray);
    void receivePacket();


private:
    QTcpSocket *mainSocket;
    QTcpSocket *fileSocket;

signals:
    void sendWaitPatient(QStringList);
};

#endif // MAINNETWORKMANAGER_H
