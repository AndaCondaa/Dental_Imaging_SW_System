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
//    void sendButtonControl(int buttonIdx, QString msg);

    explicit CBCTFileTransfer(QObject * parent = nullptr);
    ~CBCTFileTransfer();
    void sendingControl(int buttonIdx, QString msg);

    void sendPanoFile(int panoValue);
    void sendCephFile(int cephValue);

private slots:
    void receiveControl();

private:
    Protocol *protocol;

    QTcpSocket *CBCTSocket;
    QTcpSocket *fileSocket;

    QString currentPID = "NULL";
    QString currentType = "NULL";

    QByteArray panoData;
    QByteArray cephData;
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
    void panoSignal();
    void cephSignal();
};

#endif // CBCTFILETRANSFER_H
