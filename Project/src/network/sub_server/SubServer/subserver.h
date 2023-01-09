/*
 * 프로그램명 : SubServer
 * 파일명 : subserver.h
 * 설명 : 영상 검사실용 미니 서버 (촬영SW와 Modality의 연결)
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.01.06
 */

#ifndef SUBSERVER_H
#define SUBSERVER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class SubServer; }
QT_END_NAMESPACE

class QTcpServer;
class QTcpSocket;

class SubServer : public QWidget
{
    Q_OBJECT

public:
    explicit SubServer(QWidget *parent = nullptr);
    ~SubServer();

private slots:
    void newClient();
    void sendSocketToClient(QTcpSocket* sock, QString header, QString event,QString pid);
    void receiveSocketFromClient();

private:
    Ui::SubServer *ui;

    QTcpServer *server;
    QTcpSocket *sock;
};
#endif // SUBSERVER_H
