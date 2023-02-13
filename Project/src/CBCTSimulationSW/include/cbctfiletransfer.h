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
    void sendingControl(QString header, QString event, int type, QString msg);

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



signals:
    void receiveResetSignal(QString receiveMsg);
    void receiveReadySignal(QString receiveMsg);
    void receiveStartSignal(QString receiveMsg);
    void receiveStopSignal(QString receiveMsg);
    void receivePanoSignal(QString receiveMsg);
    void receiveCephSignal(QString receiveMsg);

  //  void modality_Signal(QString msg);
    void fileLogSignal(QString mode, QString fileLog);
    void sending_Control_Signal(QString msg);
};

#endif // CBCTFILETRANSFER_H
