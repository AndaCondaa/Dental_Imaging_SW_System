/*
 * 프로그램명 : ImagingSW
 * 파일명 : controlsocket.h
 * 설명 : 영상장비에 대한 제어명령 전송 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.01.06
 */

#ifndef CONTROLSOCKET_H
#define CONTROLSOCKET_H

#include <QObject>

class QTcpSocket;

class ControlSocket : public QObject
{
    Q_OBJECT
public:
    explicit ControlSocket(QObject *parent = nullptr);
    ~ControlSocket();

signals:

private slots:
    void connectToSubServer();
    void sendSocketToSubServer(QTcpSocket* sock, QString header, QString event,QString pid);      // 서브서버로 소켓전송 (명령 or Ack 메시지)
    void receiveSocketFromSubServer();

private:
    QTcpSocket *socket;
};

#endif // CONTROLSOCKET_H
