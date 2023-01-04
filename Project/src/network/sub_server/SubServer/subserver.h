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
    Test2
} Protocol;

class SubServer : public QWidget
{
    Q_OBJECT

public:
    explicit SubServer(QWidget *parent = nullptr);
    ~SubServer();

private slots:
    void newClient();
    void receiveData();

private:
    Ui::SubServer *ui;

    QTcpServer *server;
    QTcpSocket *sock;
};
#endif // SUBSERVER_H
