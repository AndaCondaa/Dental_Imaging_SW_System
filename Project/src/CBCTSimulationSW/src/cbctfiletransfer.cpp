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
        qDebug("CBCT Connected");
        connect(CBCTSocket, SIGNAL(readyRead()), this, SLOT(receiveControl()));
//        connect(CBCTSocket, SIGNAL(readyRead()), this, SLOT(receiveModality()));
        protocol->sendProtocol(CBCTSocket, "NEW", ConnectType::MODALITY, "NEW CBCT CONNECTED");
    }
    else
    {
        qDebug("CBCT Not Connected");
    }

    fileSocket = new QTcpSocket(this);
    fileSocket->connectToHost("192.168.0.20", 8003);
    if(fileSocket->waitForConnected())
    {
        qDebug("File Transfer Ready");
//        connect(fileSocket, SIGNAL(readyRead()), this, SLOT(receiveControl()));
//        protocol->sendProtocol(fileSocket, "NEW", ConnectType::MODALITY, "NEW CBCT CONNECTED");
    }
    else
    {
        qDebug("File Transfer Not Ready");
    }


//    FileSocket = new QTcpSocket(this);
//    FileSocket->connectToHost("127.0.0.1", 8009);
//    FileSocket->waitForConnected();
}
void CBCTFileTransfer::sendPanoFile()
{
    QFile file;
    QString fileName;
    QString fileInfo;

    for (int i = 10; i < 1750; i++) {
        if (i >= 100)
            fileName = QString("./Pano_Frame/0%1.raw").arg(i);
        else
            fileName = QString("./Pano_Frame/00%1.raw").arg(i);


        file.setFileName(fileName);
        file.open(QIODevice::ReadOnly);
        panoData.append(file.readAll());
        file.close();
    }
    qDebug("TOTAL SIZE : %d", panoData.size());
    fileInfo = "<CR>" + QString::number(panoData.size()) + "<CR>" + QString::number(48*2400);
    //    totalData.append(fileInfo.toStdString());
    fileSocket->write(panoData);                                       // 영상 파일 전송
    fileSocket->flush();
    panoData.clear();

    fileSocket->write(fileInfo.toStdString().data());                   // 영상 파일 정보 전송
}

void CBCTFileTransfer::sendCephFile()
{
    QFile file;
    QString fileName;
    QString fileInfo;

    for (int i = 10; i < 1250; i++) {
        if (i >= 100)
            fileName = QString("./Ceph_Frame/0%1.raw").arg(i);
        else
            fileName = QString("./Ceph_Frame/00%1.raw").arg(i);


        file.setFileName(fileName);
        file.open(QIODevice::ReadOnly);
        cephData.append(file.readAll());
        file.close();
    }
    qDebug("TOTAL SIZE : %d", cephData.size());
    fileInfo = "<CR>" + QString::number(cephData.size()) + "<CR>" + QString::number(48*2400);
    //    totalData.append(fileInfo.toStdString());
    fileSocket->write(cephData);                                       // 영상 파일 전송
    fileSocket->flush();
    cephData.clear();

    fileSocket->write(fileInfo.toStdString().data());                   // 영상 파일 정보 전송
}



//void CBCTFileTransfer::sendButtonControl(int buttonIdx, QString data)
//{
//    if (buttonIdx == 1 || buttonIdx == 2) {   // 1: RESET , 2: START
//        currentPID = data.split("|")[0];
//        currentType.split("|")[1];
//    }

//    protocol->sendProtocol(CBCTSocket, "CTL", buttonIdx, QString());
//}

void CBCTFileTransfer::sendingControl(int buttonIdx, QString msg)
{
    protocol->sendProtocol(CBCTSocket, "CTL", buttonIdx, msg);
}

void CBCTFileTransfer::receiveModality()
{
//    qDebug("modality test");
//    CBCTSocket = dynamic_cast<QTcpSocket*>(sender());
//    protocol->receiveProtocol(CBCTSocket);
//    qDebug()<<protocol->packetData()->event();
//    if(protocol->packetData()->event() == "CTL")
//    {

//        QString modality = protocol->packetData()->msg();
//        if(modality == "PANO"){
//            qDebug("Pano Modality Received");
//            emit panoSignal();
//        }
//        else if (modality == "CEPH"){
//            qDebug("Ceph Modlality Received");
//            emit cephSignal();
//        }
//        else{
//            qDebug("WRONG MODAL STATE");
//        }
//    }
}
/* 촬영 SW 에서 받은 신호를 동작을 위한 시그널로 변환 */
void CBCTFileTransfer::receiveControl()
{
    qDebug("control receive test");
    CBCTSocket = dynamic_cast<QTcpSocket*>(sender());
    protocol->receiveProtocol(CBCTSocket);

    if(protocol->packetData()->event() == "CTL")
    {

        int control = protocol->packetData()->type();
        QString modality = protocol->packetData()->msg();
        switch (control) {
        case 0:
            qDebug("RESET Received");
            emit resetSignal();
            break;
        case 1:
            qDebug("READY Received");

            if(modality == "PANO"){
                qDebug("Pano Modality Received");
                emit panoSignal();
            }
            else if (modality == "CEPH"){
                qDebug("Ceph Modlality Received");
                emit cephSignal();
            }
            else{
                qDebug("WRONG MODAL STATE");
            }
            emit readySignal();
            break;
        case 2:
            qDebug("START Received");
            emit startSignal();
            break;
        case 3:
            qDebug("STOP Received");
            emit stopSignal();
            break;
        }
    }
}

//void CBCTFileTransfer::goOnSend(qint64 numBytes)
//{
//    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
//    byteToWrite -= numBytes; // Remaining data size
//    outBlock = file->read(qMin(byteToWrite, numBytes));
//    socket->write(outBlock);

//    if (byteToWrite == 0) { // Send completed
//        qDebug("File sending completed!");
//    }
//}


CBCTFileTransfer::~CBCTFileTransfer()
{
    CBCTSocket->close();
    fileSocket->close();
    delete CBCTSocket;
    delete fileSocket;
    delete protocol;
}
//void CBCTFileTransfer::sendCTProtocol()
//{

//}
