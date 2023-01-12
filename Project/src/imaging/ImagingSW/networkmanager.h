/*
 * 프로그램명 : ImagingSW
 * 파일명 : networkmanager.h
 * 설명 : 영상장비에 대한 제어명령 전송 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.01.09
 */

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>

#include "protocol.h"

class QTcpSocket;

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

private slots:
    void connection(QString address, int port);
    void receiveSocket();
    void receiveButtonSignal(int);
    void receiveFile();


private:
    QTcpSocket *subSocket;
    QTcpSocket *fileSocket;

    Protocol *protocol;
};

#endif // NETWORKMANAGER_H
