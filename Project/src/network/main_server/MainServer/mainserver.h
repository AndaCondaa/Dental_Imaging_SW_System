#ifndef MAINSERVER_H
#define MAINSERVER_H

#include <QMainWindow>
#include <QtCore>
#include <QtNetwork>
#include <QString>

namespace Ui {
class MainServer;
}

class MainServer : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainServer(QWidget *parent = nullptr);
    ~MainServer();

signals:
    void dataReceived(QByteArray);

private slots:
    void newConnection();
    void disconnected();
    void receiveData();

private:
    Ui::MainServer *ui;
    QTcpServer *server;
    QHash<QTcpSocket*, QByteArray*> buffers; //We need a buffer to store data until block has completely received
    QHash<QTcpSocket*, qint32*> sizes; //We need to store the size to verify if a block has received completely
};
#endif // MAINSERVER_H
