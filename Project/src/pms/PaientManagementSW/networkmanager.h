#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QtCore>
#include <QtNetwork>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    NetworkManager(QObject *parent = nullptr);

public slots:
    bool connectToHost(QString host);
    bool writeData(QByteArray data);

private slots:
    void newDataSended(QString);

private:
    QTcpSocket *socket;
    bool fd_flag = false;
    bool send_flag = false;
    QTcpSocket *PMSocket;

};

#endif // NETWORKMANAGER_H
