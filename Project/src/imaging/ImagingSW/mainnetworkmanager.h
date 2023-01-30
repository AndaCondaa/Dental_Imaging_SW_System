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
    void connection(QString address, int port);
    void sendPacket(QTcpSocket* socket, QString event, QString pid, QString data);
    QStringList packetParser(QByteArray receiveArray);
    void receivePacket();

    void requestPatientInfo(QString pid);       // 환자매니저에서 환자준비버튼 눌렀을 때, 서버로 패킷전송
    void endImagingProcess(QString pid);        // 이미징매니저에서 환자 촬영이 모두 끝난 경우, 서버로 패킷전송


    void goOnSend(qint64 numBytes);
    void sendFile(QString data);            // data = pid | shoot_type


private:
    QTcpSocket *mainSocket;
    QTcpSocket *fileSocket;

    QFile* file;                                // File Object for FileSending Protocol
    qint64 totalSize;                           // Total size of File that clients are sending
    qint64 byteReceived = 0;                    // size of File read currently
    QByteArray inBlock;                         // Units divided to transfer files
    QString fileName;                           // Receiving FileName
    QString fileSender;                         // Receiving File's Sender
    QString checkFileName;                      // Previous File Name for checking new file
    qint64 loadSize;                // File Size
    qint64 byteToWrite;             // File Size per a block
    QByteArray outBlock;            // Block for sending

signals:
    void sendWaitPatient(QStringList);
    void sendPatientInfo(QStringList);
};

#endif // MAINNETWORKMANAGER_H
