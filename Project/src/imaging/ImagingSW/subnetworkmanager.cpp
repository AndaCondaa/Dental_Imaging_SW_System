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

    connection("127.0.0.1", 8002);
}

SubNetworkManager::~SubNetworkManager()
{
    subSocket->close();
    delete subSocket;
    delete protocol;
}

void SubNetworkManager::connection(QString address, int port)
{
    subSocket->connectToHost(address, port);
    if (subSocket->waitForConnected()) {
        connect(subSocket, SIGNAL(readyRead()), SLOT(receiveControl()));
        protocol->sendProtocol(subSocket, "NEW", ConnectType::SW, "SW");
    } else {
        // 연결 실패 예외처리 구현
    }

    fileSocket->connectToHost(address, port+1);
    if (fileSocket->waitForConnected()) {
        connect(fileSocket, SIGNAL(readyRead()), SLOT(receiveFile()));
        protocol->sendProtocol(fileSocket, "NEW", ConnectType::SW, "SW");
    } else {
        // 연결 실패  예외처리 구현
    }
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
    if (buttonIdx == 1 || buttonIdx == 2) {   // 1: RESET , 2: START
        currentPID = data.split("|")[0];
        currentType = data.split("|")[1];
    }

    protocol->sendProtocol(subSocket, "CTL", buttonIdx, data);
}

void SubNetworkManager::receiveFile()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());

    totalData.append(socket->readAll());


    if (totalData.size() == 227865600) {
        QFile file;
        QString fileName;

        QDir dir(QString("receive/%1/%2").arg(currentPID, currentType));
        if (!dir.exists())
            dir.mkpath(".");

        int count = 0;
        for (int i = 10; i < 999; i++) {
            if (i >= 100)
                fileName = dir.path() + "/" + QString("0%1.raw").arg(i);
            else
                fileName = dir.path() + "/" + QString("00%1.raw").arg(i);

            file.setFileName(fileName);
            file.open(QIODevice::WriteOnly);

            file.write(totalData.mid(count*48*2400*2, 48*2400*2));
            qDebug("%d개 저장 완료", count);

            file.close();
            count++;
        }
        qDebug("DONE!!!! (line: %d)", __LINE__);
    }
}
