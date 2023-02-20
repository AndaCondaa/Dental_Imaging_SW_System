/*
 * 프로그램명 : ImagingSW
 * 파일명 : mainnetworkmanager.h
 * 설명 : 메인 서버와 통신 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.13
 */


#ifndef MAINNETWORKMANAGER_H
#define MAINNETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QProgressDialog>

class MainNetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit MainNetworkManager(QObject *parent = nullptr);
    ~MainNetworkManager();

private slots:
    void connectServer(QString address, int port);                  // 메인서버 연결
    void disconnectServer();                                        // 서버 연결 해제

    void sendPacket(QTcpSocket* socket, QString header,
                    QString event, QString pid, QString data);      // 패킷 구성 및 전송
    QStringList packetParser(QByteArray receiveArray);              // 수신 패킷 파싱 후 저장
    void receivePacket();                                           // 수신된 패킷에 따른 동작

    void requestPatientInfo(QString pid);                           // 환자정보 요청
    void endImagingProcess(QString pid, QString type);              // 촬영종료 노티

    void sendingFile(qint64 numBytes);                              // 실제 파일내용 전송
    void sendFile(QString data);                                    // 파일헤더 전송 (data: pid|shoot_type)

private:
    QTcpSocket *mainSocket = nullptr;       // 정보전달 소켓
    QTcpSocket *fileSocket = nullptr;       // 파일전송 소켓

    QFile* file = nullptr;                  // 파일 전송용 파일클래스 객체
    QString fileName;                       // 전송파일 이름
    qint64 fileSize;                        // 전송파일 사이즈
    qint64 remainBytes;                     // 전송된 바이트
    QByteArray sendingArray;                // 전송하는 패킷 내용

signals:
    void connectionStatusChanged(bool);     // 서버연결 상태 변경
    void sendWaitList(int, QString);        // 기존 대기목록 전송
    void sendWaitPatient(QStringList);      // 촬영요청 정보 전송
    void sendPatientInfo(QStringList);      // 요청한 환자정보 전송
};

#endif // MAINNETWORKMANAGER_H
