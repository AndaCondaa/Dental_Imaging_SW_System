#ifndef CBCTFILETRANSFER_H
#define CBCTFILETRANSFER_H

#include <QTcpSocket>
#include <QDataStream>
#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
class Protocol;

class CBCTFileTransfer : public QObject
{
    Q_OBJECT
public:
    explicit CBCTFileTransfer(QObject * parent = nullptr);
    ~CBCTFileTransfer();

private slots:
    void sendFile();
    void sendControl(int buttonIdx);
    void receiveControl();
    void goOnSend(qint64 numBytes);

    //    void sendCBCTProtocol();
    //    void sendRawImages();
    //    void disconnect();

private:
    Protocol *protocol;
    QTcpSocket *CBCTSocket;

    QString currentPID = "NULL";
    QString currentType = "NULL";



    QTcpSocket *subSocket;
    QTcpSocket *fileSocket;

    QFile* file;
    qint64 loadSize;                            // File Object for FileSending Protocol
    qint64 totalSize;                           // Total size of File that clients are sending
    qint64 byteToWrite = 0;
    QByteArray outBlock;                        // size of File read currently
    QByteArray inBlock;                         // Units divided to transfer files
    QString fileName;                           // Receiving FileName
    QString fileSender;                         // Receiving File's Sender
    QString checkFileName;                      // Previous File Name for checking new file
    //    QTcpSocket * FileSocket;



signals:
    void resetSignal();
    void readySignal();
    void startSignal();
    void stopSignal();
};

#endif // CBCTFILETRANSFER_H
