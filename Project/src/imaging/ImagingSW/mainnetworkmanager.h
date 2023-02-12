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
    void connectSever(QString address, int port);
    void disconnectServer();

    void sendPacket(QTcpSocket* socket, QString header, QString event, QString pid, QString data);
    QStringList packetParser(QByteArray receiveArray);
    void receivePacket();

    void requestPatientInfo(QString pid);       // 환자매니저에서 환자준비버튼 눌렀을 때, 서버로 패킷전송
    void endImagingProcess(QString pid, QString type);        // 이미징매니저에서 환자 촬영이 모두 끝난 경우, 서버로 패킷전송

    void goOnSend(qint64 numBytes);
    void sendFile(QString data);            // data = pid | shoot_type

private:
    QTcpSocket *mainSocket = nullptr;
    QTcpSocket *fileSocket = nullptr;

    QFile* file = nullptr;                                // File Object for FileSending Protocol
    qint64 totalSize;                           // Total size of File that clients are sending
    qint64 byteReceived = 0;                    // size of File read currently
    QByteArray inBlock;                         // Units divided to transfer files
    QString fileName;                           // Receiving FileName
    qint64 byteToWrite;             // File Size per a block
    QByteArray outBlock;            // Block for sending
    QMap<int, QTcpSocket*> socketMap; // 0: main , 1: file

signals:
    void connectionStatusChanged(bool);
    void sendWaitList(int, QString);
    void sendWaitPatient(QStringList);
    void sendPatientInfo(QStringList);
};

#endif // MAINNETWORKMANAGER_H
