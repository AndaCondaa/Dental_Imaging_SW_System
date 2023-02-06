/*
 * 프로그램명 : ImagingSW
 * 파일명 : networkmanager.h
 * 설명 : 영상장비에 대한 제어명령 전송 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.01.09
 */

#ifndef SUBNETWORKMANAGER_H
#define SUBNETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QFile>
#include <QFileInfo>

class QTcpSocket;
class Protocol;

class SubNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit SubNetworkManager(QObject *parent = nullptr);
    ~SubNetworkManager();

private slots:
    void receiveControl();
    void sendButtonControl(int, QString);
    void receiveFile();

private:
    void connection(QString address, int port);

    QString currentPID = "NULL";
    QString currentType = "NULL";

    Protocol *protocol;

    QTcpSocket *subSocket;
    QTcpSocket *fileSocket;

    int count = 0;
    int countMax = 0;
    int frameSize = 0;
    QByteArray totalData;

signals:
    void buttonSignal(int);         // 영상 장비로부터 직접 명령이 왔을 때 발생
    void sendFrameImg(int);
};

#endif // SUBNETWORKMANAGER_H
