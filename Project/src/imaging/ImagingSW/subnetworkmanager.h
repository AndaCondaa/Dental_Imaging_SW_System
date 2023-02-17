/*
 * 프로그램명 : ImagingSW
 * 파일명 : subnetworkmanager.h
 * 설명 : 영상장비에 대한 제어명령 전송 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.13
 */

#ifndef SUBNETWORKMANAGER_H
#define SUBNETWORKMANAGER_H

#include <QObject>

class Protocol;
class QTcpSocket;

class SubNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit SubNetworkManager(QObject *parent = nullptr);
    ~SubNetworkManager();

private slots:
    void connectServer(QString address, int port);          // 서브서버 연결
    void disconnectServer();                                // 서버 연결 해제

    void receiveControl();                                  // 장비제어 패킷 리시브
    void sendButtonControl(int, QString);                   // 장비제어 패킷 전송

    void receiveFile();                                     // 프레임데이터 리시브

private:
    Protocol *protocol = nullptr;

    QTcpSocket *subSocket = nullptr;                        // 장비제어용 소켓
    QTcpSocket *fileSocket = nullptr;                       // 프레임데이터 전송용 소켓

    QString currentPID = "NULL";                            // 현재 촬영중인 환자번호
    QString currentType = "NULL";                           // 촬영하려는 타입 (PANO, CEPH)

    int count = 0;                          // 프레임데이터 전송 수 카운트
    int countMax = 0;                       // 타입에 따른 전송될 프레임데이터 수 (1750,1250)
    int frameSize = 0;                      // 타입에 따른 프레임데이터 한 장 사이즈
    QByteArray totalData;                   // 전송받은 일련의 데이터

signals:
    void connectionStatusChanged(bool);     // 서버 연결 상태 변경
    void buttonSignal(int);                 // 제어명령 발생
    void sendFrameImg(int);                 // 수신된 프레임데이터마다 카운트 전송
};

#endif // SUBNETWORKMANAGER_H
