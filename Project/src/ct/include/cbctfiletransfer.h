#ifndef CBCTFILETRANSFER_H
#define CBCTFILETRANSFER_H

#include <QTcpSocket>
#include <QDataStream>
#include <QObject>

class CBCTFileTransfer : public QObject
{
    Q_OBJECT
public:
    CBCTFileTransfer();

private:
    QTcpSocket * CBCTSocket;
    QTcpSocket * FileSocket;
};

#endif // CBCTFILETRANSFER_H
