#include "cbctfiletransfer.h"

CBCTFileTransfer::CBCTFileTransfer()
{
    CBCTSocket = new QTcpSocket(this);
    CBCTSocket->connectToHost("127.0.0.1", 8009);
    CBCTSocket->waitForConnected();

    FileSocket = new QTcpSocket(this);
    FileSocket->connectToHost("127.0.0.1", 8009);
    FileSocket->waitForConnected();
}
