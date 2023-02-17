#include "cbctfiletransfer.h"
#include "protocol.h"
#include "packetdata.h"
#include "qmessagebox.h"


CBCTFileTransfer::CBCTFileTransfer(QObject*parent):QObject{parent}
{
    protocol = new Protocol();
    CBCTSocket = new QTcpSocket(this);
    CBCTSocket->connectToHost("192.168.0.97", 8002);
    if(CBCTSocket->waitForConnected(100))
    {
        qDebug("CBCT Connected");
        connect(CBCTSocket, SIGNAL(readyRead()), this, SLOT(receiveControl()));
        protocol->sendProtocol(CBCTSocket, "SEN", "NEW", ConnectType::MODALITY, "NEW CBCT CONNECTED");
    }
    //sendProtocol(QTcpSocket* socket, QString header, QString event, int type, QString msg);
    else
    {
        qDebug("CBCT Not Connected");
    }

    fileSocket = new QTcpSocket(this);
    fileSocket->connectToHost("192.168.0.97", 8003);
    if(fileSocket->waitForConnected(100))
    {
        qDebug("File Transfer Ready");
        connect(fileSocket, SIGNAL(readyRead()), this, SLOT(receiveControl()));
        protocol->sendProtocol(fileSocket, "SEN", "NEW", ConnectType::MODALITY, "CBCT FILE TRANSFER READY");
    }
    else
    {
        qDebug("File Transfer Not Ready");
    }
}

void CBCTFileTransfer::connectSubServer(QString address, int port)
{
    CBCTSocket = new QTcpSocket(this);
    fileSocket = new QTcpSocket(this);

    CBCTSocket->connectToHost(address, port);
    fileSocket->connectToHost(address, port+1);

    if (CBCTSocket->waitForConnected(1000) && fileSocket->waitForConnected(1000)) {
        connect(CBCTSocket, SIGNAL(readyRead()), SLOT(receiveControl()));
        connect(CBCTSocket, SIGNAL(disconnected()), this, SLOT(disconnectServer()));
        connect(fileSocket, SIGNAL(readyRead()), SLOT(receiveFile()));

        protocol->sendProtocol(CBCTSocket, "SEN", "NEW", ConnectType::MODALITY, "NEW CBCT CONNECTED");
        protocol->sendProtocol(fileSocket, "SEN", "NEW", ConnectType::MODALITY, "CBCT FILE TRANSFER READY");

        emit connectStatusChanged(true);
    } else {
        QMessageBox disconnectBox(QMessageBox::Warning, "ERROR",
                                  "Not connected",
                                  QMessageBox::Ok);
        disconnectBox.exec();
        return;
    }
}

void CBCTFileTransfer::disconnectSubServer()
{
    CBCTSocket->close();
       fileSocket->close();

       CBCTSocket->deleteLater();
       fileSocket->deleteLater();

       QMessageBox disconnectBox(QMessageBox::Warning, "ERROR",
                                 "Not connected",
                                 QMessageBox::Ok);
       disconnectBox.exec();

       emit connectStatusChanged(false);
}
void CBCTFileTransfer::sendPanoFile(int panoValue)
{
    QString modality = protocol->packetData()->msg();

    qDebug() << protocol->packetData()->msg();

     int countMax = 1750;

    // PANO MODE
        QFile *panoFile;
        panoFile = new QFile;
        QString panoFileName;
   
        if (protocol->packetData()->header() == "ACK")
        {
    if(panoValue >= countMax)
    {
        return;
    }
    if (panoValue >= 1000)
    {
    //    qDebug() << panoValue;
        panoFileName = QString("C:/Qt_VTK_CT/RawImages/Pano_Frame(1152x64)/%1.raw").arg(panoValue);
    }
    else if (panoValue < 1000 && panoValue >= 100)
    {
   //     qDebug() << panoValue;
        panoFileName = QString("C:/Qt_VTK_CT/RawImages/Pano_Frame(1152x64)/0%1.raw").arg(panoValue);
    }
    else if (panoValue < 100 && panoValue >= 10)
    {
   //     qDebug() << panoValue;
        panoFileName = QString("C:/Qt_VTK_CT/RawImages/Pano_Frame(1152x64)/00%1.raw").arg(panoValue);
    }
    else
    {
   //     qDebug() << panoValue;
        panoFileName = QString("C:/Qt_VTK_CT/RawImages/Pano_Frame(1152x64)/000%1.raw").arg(panoValue);
    }
        }
        else if (protocol->packetData()->header() == "ERROR")
        {
            qDebug("Pano File Transfer Error State");
        }
    panoFile->setFileName(panoFileName);
  //  qDebug() << panoFileName;
    if(!panoFile->exists())
    {
        qDebug() << " not file Exist : " << panoFileName;
        return;
    }

    bool panoOpen = panoFile->open(QIODevice::ReadOnly);
    fileSocket->write(panoFile->readAll());
    qDebug() << "value : " << panoValue << "File name : " << panoFileName;
    if(panoOpen)
    {
    emit fileLogSignal("PANORAMA MODE", panoFileName);
    }
    else
    {
    emit fileLogSignal("No Panorama Files", panoFileName);
    }

    panoFile->close();
    panoFile->deleteLater();
}

void CBCTFileTransfer::sendCephFile(int cephValue)
{
    QString modality = protocol->packetData()->msg();

    qDebug() << protocol->packetData()->msg();

int countMax = 1250;
    

    // CEPH MODE
        QFile* cephFile;
        cephFile = new QFile;
        QString cephFileName;
        if (protocol->packetData()->header() == "ACK")
        {
            if (cephValue >= countMax)
            {
                return;
            }
            if (cephValue >= 1000)
            {
                qDebug() << cephValue;
                cephFileName = QString("C:/Qt_VTK_CT/RawImages/Ceph_Frame(48x2400)/%1.raw").arg(cephValue);
            }
            else if (cephValue < 1000 && cephValue >= 100)
            {
                qDebug() << cephValue;
                cephFileName = QString("C:/Qt_VTK_CT/RawImages/Ceph_Frame(48x2400)/0%1.raw").arg(cephValue);
            }
            else if (cephValue < 100 && cephValue >= 10)
            {
                qDebug() << cephValue;
                cephFileName = QString("C:/Qt_VTK_CT/RawImagesCeph_Frame(48x2400)/00%1.raw").arg(cephValue);
            }
            else
            {
                qDebug() << cephValue;
                cephFileName = QString("C:/Qt_VTK_CT/RawImages/Ceph_Frame(48x2400)/000%1.raw").arg(cephValue);
            }
        }
        else if(protocol->packetData()->header() == "ERROR")
        {
            qDebug("Ceph File Transfer Error State");
        }
    cephFile->setFileName(cephFileName);
    if(!cephFile->exists())
    {
        qDebug() << " not file Exist : " << cephFileName;
        return;
    }
    bool cephOpen = cephFile->open(QIODevice::ReadOnly);
    fileSocket->write(cephFile->readAll());
    qDebug() << "value : " << cephValue << "File name : " << cephFileName;
    /* 전송되는 이미지를 테이블위젯에 출력 */
    if(cephOpen)
    {
    emit fileLogSignal("CEPHALO MODE", cephFileName);
    }
    else
    {
    emit fileLogSignal("No Cephalo Files", cephFileName);
    }

    cephFile->close();
    cephFile->deleteLater();
}

void CBCTFileTransfer::sendingControl(QString header, QString event, int type, QString msg)
{
    protocol->sendProtocol(CBCTSocket, "SEN", "CTL", type, msg);

    emit sending_Control_Signal(msg);
}

void CBCTFileTransfer::receiveControl()
{
    qDebug("control receive test");
    CBCTSocket = dynamic_cast<QTcpSocket*>(sender());
    protocol->receiveProtocol(CBCTSocket);

    if(protocol->packetData()->event() == "CTL")
    {
        if (protocol->packetData()->header() == "ACK")
        {
            int control = protocol->packetData()->type();
            QString modality = protocol->packetData()->msg();

            switch (control) {
            case 0:
                qDebug("RESET Received");
                emit receiveResetSignal("RESET Received");
                break;
            case 1:
                qDebug("READY Received");

                if (modality == "PANO") {
                    qDebug("Pano Modality Received");
                    emit receivePanoSignal("Pano Modality Received");
                }
                else if (modality == "CEPH") {
                    qDebug("Ceph Modlality Received");
                    emit receiveCephSignal("Ceph Modlality Received");
                }
                else {
                    qDebug("WRONG MODAL STATE");
                }
                emit receiveReadySignal("READY Received");
                break;
            case 2:
                qDebug("START Received");
                emit receiveStartSignal("START Received");
                break;
            case 3:
                qDebug("STOP Received");
                emit receiveStopSignal("STOP Received");
                break;
            }
        }
        else if (protocol->packetData()->header() == "ERROR")
        {
            qDebug("Receive Control Error State");
        }
    }
}


CBCTFileTransfer::~CBCTFileTransfer()
{
    CBCTSocket->close();
    fileSocket->close();
    delete CBCTSocket;
    delete fileSocket;
    delete protocol;
}
