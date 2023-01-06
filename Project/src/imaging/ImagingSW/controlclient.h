#ifndef CONTROLCLIENT_H
#define CONTROLCLIENT_H

#include <QObject>

class QTcpSocket;

class SubClient : public QObject
{
    Q_OBJECT
public:
    explicit SubClient(QObject *parent = nullptr);
    ~SubClient();

signals:

private:
    QTcpSocket *socket;
};

#endif // CONTROLCLIENT_H
