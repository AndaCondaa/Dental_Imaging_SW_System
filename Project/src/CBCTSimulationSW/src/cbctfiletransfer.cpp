#include "cbctfiletransfer.h"
#include "protocol.h"
#include "packetdata.h"

CBCTFileTransfer::CBCTFileTransfer(QObject*parent):QObject{parent}
{
    protocol = new Protocol();
    CBCTSocket = new QTcpSocket(this);
    CBCTSocket->connectToHost("192.168.0.20", 8002);
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
        int control = protocol->packetData()->type();
        switch (control) {
        case 0:
            emit resetSignal();
            break;
        case 1:
            emit readySignal();
            break;
        case 2:
            emit startSignal();
            break;
        case 3:
            emit stopSignal();
            break;
        }
    }

}

void CBCTFileTransfer::goOnSend(qint64 numBytes)
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    byteToWrite -= numBytes; // Remaining data size
    outBlock = file->read(qMin(byteToWrite, numBytes));
    socket->write(outBlock);

    if (byteToWrite == 0) { // Send completed
        qDebug("File sending completed!");
    }
}


void CBCTFileTransfer::sendFile()
{
    loadSize = 0;
    byteToWrite = 0;
    totalSize = 0;
    outBlock.clear();

    QString filename = QFileDialog::getOpenFileName();
    if(filename.length()) {
        file = new QFile(filename);
        file->open(QFile::ReadOnly);

        qDebug() << QString("file %1 is opened").arg(filename);

        byteToWrite = totalSize = file->size(); // Data remained yet
        loadSize = 1024; // Size of data per a block

        QDataStream out(&outBlock, QIODevice::WriteOnly);
        out << qint64(0) << qint64(0) << filename;

        totalSize += outBlock.size();
        byteToWrite += outBlock.size();

        out.device()->seek(0);
        out << totalSize << qint64(outBlock.size());

        fileSocket->write(outBlock); // Send the read file to the socket
    }
    qDebug() << QString("Sending file %1").arg(filename);
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
