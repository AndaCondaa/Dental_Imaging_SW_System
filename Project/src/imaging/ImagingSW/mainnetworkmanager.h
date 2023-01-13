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
    void receivePacket();

private:
    QTcpSocket *mainSocket;
    QTcpSocket *fileSocket;

signals:

};

#endif // MAINNETWORKMANAGER_H
