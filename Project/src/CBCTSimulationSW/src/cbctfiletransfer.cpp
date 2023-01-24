#include "cbctfiletransfer.h"
#include "protocol.h"
#include "packetdata.h"

CBCTFileTransfer::CBCTFileTransfer(QObject*parent):QObject{parent}
{
    protocol = new Protocol();
    CBCTSocket = new QTcpSocket(this);
    CBCTSocket->connectToHost("112.221.225.166", 8002);
    if(CBCTSocket->waitForConnected())
    {
        qDebug("connecting");
        connect(CBCTSocket, SIGNAL(readyRead()), this, SLOT(receiveControl()));
        protocol->sendProtocol(CBCTSocket, "NEW", ConnectType::MODALITY, "NEW CBCT CONNECTED");
    }
    else
    {
        qDebug("fffff");
    }

//    FileSocket = new QTcpSocket(this);
//    FileSocket->connectToHost("127.0.0.1", 8009);
//    FileSocket->waitForConnected();
}


void CBCTFileTransfer::sendControl(int buttonIdx)
{

    protocol->sendProtocol(CBCTSocket, "CTL", buttonIdx, "Pano or Ceph");

}

void CBCTFileTransfer::receiveControl()
{
    CBCTSocket = dynamic_cast<QTcpSocket*>(sender());
    protocol->receiveProtocol(CBCTSocket);

    if(protocol->packetData()->event() == "CTL")
    {
        emit sendButtonSignal(protocol->packetData()->type());
    }

}

void CBCTFileTransfer::goOnSend(qint64 numBytes)
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
//    byteToWrite -= numBytes; // Remaining data size
//    outBlock = file->read(qMin(byteToWrite, numBytes));
//    socket->write(outBlock);

//    progressDialog->setMaximum(totalSize);
//    progressDialog->setValue(totalSize-byteToWrite);

//    if (byteToWrite == 0) { // Send completed
//        qDebug("File sending completed!");
//        progressDialog->reset();
    }

CBCTFileTransfer::~CBCTFileTransfer()
{
    CBCTSocket->close();
    delete CBCTSocket;
    delete protocol;
}
//void CBCTFileTransfer::sendCTProtocol()
//{

//}
