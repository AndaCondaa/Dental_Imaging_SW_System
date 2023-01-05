#ifndef SUBSERVER_H
#define SUBSERVER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class SubServer; }
QT_END_NAMESPACE

class QTcpServer;
class QTcpSocket;

typedef enum {
    Test1,
} Protocol_Type;

class SubServer : public QWidget
{
    Q_OBJECT

public:
    explicit SubServer(QWidget *parent = nullptr);
    ~SubServer();

    void sendProtocol(QTcpSocket* sock, Protocol_Type type, QString data, int size);

private slots:
    void newClient();
    void receiveData();

private:
    Ui::SubServer *ui;

    QTcpServer *server;
    QTcpSocket *sock;
};
#endif // SUBSERVER_H
