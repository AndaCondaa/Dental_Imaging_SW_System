#ifndef CBCTFILETRANSFER_H
#define CBCTFILETRANSFER_H

#include <QTcpSocket>
#include <QDataStream>
#include <QObject>
#include <QFile>
#include <QFileInfo>

class Protocol;

class CBCTFileTransfer : public QObject
{
    Q_OBJECT
public:
    explicit CBCTFileTransfer(QObject * parent = nullptr);
    ~CBCTFileTransfer();

private slots:
    void sendControl(int buttonIdx);
    void receiveControl();
    void goOnSend(qint64 numBytes);

//    void sendCBCTProtocol();
//    void sendRawImages();
//    void disconnect();

private:
    Protocol *protocol;
    QTcpSocket *CBCTSocket;
//    QTcpSocket * FileSocket;



signals:
    void sendButtonSignal(int);
};

#endif // CBCTFILETRANSFER_H
