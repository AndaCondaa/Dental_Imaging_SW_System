/*
 * 프로그램명 : ImagingSW
 * 파일명 : networkmanager.cpp
 * 설명 : 영상장비에 대한 제어명령 전송 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.01.09
 */

#include "subnetworkmanager.h"
#include "protocol.h"
#include "packetdata.h"

#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QAbstractButton>


SubNetworkManager::SubNetworkManager(QObject *parent)
    : QObject{parent}
{
    subSocket = new QTcpSocket(this);
    fileSocket= new QTcpSocket(this);

    protocol = new Protocol();
}

SubNetworkManager::~SubNetworkManager()
{
    subSocket->deleteLater();
    delete protocol;
}

void SubNetworkManager::connectServer(QString address, int port)
{
    subSocket = new QTcpSocket(this);
    fileSocket= new QTcpSocket(this);

    subSocket->connectToHost(address, port);
    fileSocket->connectToHost(address, port+1);

    if (subSocket->waitForConnected(1000) && fileSocket->waitForConnected(1000)) {
        connect(subSocket, SIGNAL(readyRead()), SLOT(receiveControl()));
        connect(subSocket, SIGNAL(disconnected()), this, SLOT(disconnectServer()));
        connect(fileSocket, SIGNAL(readyRead()), SLOT(receiveFile()));

        protocol->sendProtocol(subSocket, "SEN", "NEW", ConnectType::SW, "SW");
        protocol->sendProtocol(fileSocket, "SEN", "NEW", ConnectType::SW, "SW");

        emit connectionStatusChanged(true);
    } else {
        qDebug("SUB Connection FAIL!");
    }
}

void SubNetworkManager::disconnectServer()
{
    subSocket->close();
    fileSocket->close();

//    subSocket->deleteLater();
//    fileSocket->deleteLater();

    emit connectionStatusChanged(false);

    QMessageBox disconnectBox(QMessageBox::Warning, "ERROR",
                              "검사실 서버와 연결이 끊어졌습니다. 재접속 해주세요.",
                              QMessageBox::Ok);
    disconnectBox.exec();
}

void SubNetworkManager::receiveControl()
{
    subSocket = dynamic_cast<QTcpSocket*>(sender());
    protocol->receiveProtocol(subSocket);

    if (protocol->packetData()->header() == "ERR") {
        QMessageBox disconnectCT(QMessageBox::Warning, "ERROR",
                                  "CT 장비가 연결되어 있지 않습니다.",
                                  QMessageBox::Ok);
        disconnectCT.exec();
    } else if (protocol->packetData()->header() == "ACK") {
        if (protocol->packetData()->event() == "CTL") {
            emit buttonSignal(protocol->packetData()->type());
        }
    }
}

void SubNetworkManager::sendButtonControl(int buttonIdx, QString data)
{
    if (buttonIdx == 1 || buttonIdx == 2) {   // 1: RESET , 2: READY
        currentPID = data.split("|")[0];
        currentType = data.split("|")[1];

        if (currentType == "PANO") {
            count = 0;
            countMax = 1750;
            frameSize = 1152 * 64 * 2;
        } else if (currentType == "CEPH") {
            count = 0;
            countMax = 1250;
            frameSize = 48 * 2400 * 2;
        }
    }

    if (subSocket == nullptr) {
        emit connectionStatusChanged(false);

        QMessageBox disconnectBox(QMessageBox::Warning, "ERROR",
                                  "서버와 연결이 끊어졌습니다. 재접속 해주세요.",
                                  QMessageBox::Ok);
        disconnectBox.exec();
    }

    protocol->sendProtocol(subSocket, "SEN", "CTL", buttonIdx, data);
}

void SubNetworkManager::receiveFile()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    totalData.append(socket->readAll());

    if (totalData.size() >= frameSize) {
        QFile file;
        QString fileName;
        QByteArray frameData = totalData.first(frameSize);

        QDir dir(QString("image/frame/%1/").arg(currentType));
        if (!dir.exists())
            dir.mkpath(".");

        if (count >= 1000)
            fileName = dir.path() + QString("/%1.raw").arg(count);
        else if (count < 1000 && count >= 100)
            fileName = dir.path() + QString("/0%1.raw").arg(count);
        else if (count < 100 && count >= 10)
            fileName = dir.path() + QString("/00%1.raw").arg(count);
        else
            fileName = dir.path() + QString("/000%1.raw").arg(count);

        file.setFileName(fileName);
        file.open(QIODevice::WriteOnly);
        file.write(frameData);
        file.close();
        totalData.remove(0, frameSize);
        emit sendFrameImg(count);
        count++;
        qDebug("%d", count);
        if (count == countMax) {
            qDebug() << QString("%1 Frame Data Send End!").arg(currentType);
            count = 0;
            totalData.clear();
            return;
        }
    }
}

