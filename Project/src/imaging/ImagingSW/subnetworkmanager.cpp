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

#include <QDir>

SubNetworkManager::SubNetworkManager(QObject *parent)
    : QObject{parent}
{
    subSocket = new QTcpSocket(this);
    fileSocket = new QTcpSocket(this);
    protocol = new Protocol();

    connection("10.222.0.164", 8002);
}

SubNetworkManager::~SubNetworkManager()
{
    subSocket->close();
    delete subSocket;
    delete protocol;
}

void SubNetworkManager::connection(QString address, int port)
{
//    subSocket->connectToHost(address, port);
//    if (subSocket->waitForConnected()) {
//        connect(subSocket, SIGNAL(readyRead()), SLOT(receiveControl()));
//        protocol->sendProtocol(subSocket, "NEW", ConnectType::SW, "SW");
//    } else {
//        // 연결 실패 예외처리 구현
//    }

//    fileSocket->connectToHost(address, port+1);
//    if (fileSocket->waitForConnected()) {
//        connect(fileSocket, SIGNAL(readyRead()), SLOT(receiveFile()));
//        protocol->sendProtocol(fileSocket, "NEW", ConnectType::SW, "SW");
//    } else {
//        // 연결 실패  예외처리 구현
//    }
}

void SubNetworkManager::receiveControl()
{
    subSocket = dynamic_cast<QTcpSocket*>(sender());
    protocol->receiveProtocol(subSocket);

    if (protocol->packetData()->event() == "CTL") {
        emit buttonSignal(protocol->packetData()->type());
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

    protocol->sendProtocol(subSocket, "CTL", buttonIdx, data);
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
