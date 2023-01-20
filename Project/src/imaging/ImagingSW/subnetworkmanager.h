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

    Protocol *protocol;

    QTcpSocket *subSocket;
    QTcpSocket *fileSocket;

    QFile* file;                                // File Object for FileSending Protocol
    qint64 totalSize;                           // Total size of File that clients are sending
    qint64 byteReceived = 0;                    // size of File read currently
    QByteArray inBlock;                         // Units divided to transfer files
    QString fileName;                           // Receiving FileName
    QString fileSender;                         // Receiving File's Sender
    QString checkFileName;                      // Previous File Name for checking new file


signals:
    void buttonSignal(int);         // 영상 장비로부터 직접 명령이 왔을 때 발생
};

#endif // SUBNETWORKMANAGER_H
