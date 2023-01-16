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

SubNetworkManager::SubNetworkManager(QObject *parent)
    : QObject{parent}
{
    subSocket = new QTcpSocket(this);
    fileSocket = new QTcpSocket(this);
    protocol = new Protocol();

    connection("127.0.0.1", 8000);
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

void SubNetworkManager::receiveButtonControl(int buttonIdx)
{
    protocol->sendProtocol(subSocket, "CTL", buttonIdx, "Pano or Ceph");
}

void SubNetworkManager::receiveFile()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());

    // Beginning File Transfer
    if (byteReceived == 0) {        // First Time(Block) , var byteReceived is always zero
        checkFileName = fileName;
        QDataStream in(socket);
        in.device()->seek(0);
        in >> totalSize >> byteReceived >> fileName >> fileSender;
        if(checkFileName == fileName) return;

        QFileInfo info(fileName);
        QString currentFileName = info.fileName();
        file = new QFile(currentFileName);
        file->open(QFile::WriteOnly);
    } else {
        if(checkFileName == fileName) return;
        inBlock = socket->readAll();

        byteReceived += inBlock.size();
        file->write(inBlock);
        file->flush();
    }

    if (byteReceived == totalSize) {        // file sending is done
        qDebug() << QString("%1 receive completed").arg(fileName);
        inBlock.clear();
        byteReceived = 0;
        totalSize = 0;
        file->close();
        delete file;
    }
}