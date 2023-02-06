/*
 * 프로그램명 : SubServer
 * 파일명 : subserver.cpp
 * 설명 : 영상 검사실용 미니 서버 (촬영SW와 Modality의 연결)
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.01.06
 */

#include "subserver.h"
#include "ui_subserver.h"

#include "protocol.h"
#include "packetdata.h"

#include <QDir>

SubServer::SubServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubServer)
{
    ui->setupUi(this);

    protocol = new Protocol();

    // 장비제어 명령 서버 오픈
    controlServer = new QTcpServer();
    connect(controlServer, SIGNAL(newConnection()), this, SLOT(newClient()));
    if(!controlServer->listen(QHostAddress::Any, 8002)) {
        // 서버 listen 실패
    }

    // 이미지 서버 오픈
    fileServer = new QTcpServer();
    connect(fileServer, SIGNAL(newConnection()), this, SLOT(newFileClient()));
    if(!fileServer->listen(QHostAddress::Any, 8003)) {
        // 파일 서버 listen 실패
    }
}

SubServer::~SubServer()
{
    delete ui;
    controlServer->close();
    fileServer->close();
    delete controlServer;
    delete protocol;
}

void SubServer::newClient()
{
    QTcpSocket *newSocket = controlServer->nextPendingConnection();
    connect(newSocket, SIGNAL(readyRead()), this, SLOT(receiveControl()));
}

void SubServer::newFileClient()
{
    QTcpSocket *fileSocket = fileServer->nextPendingConnection();
    connect(fileSocket, SIGNAL(readyRead()), this, SLOT(firstFileSocket()));
}

void SubServer::firstFileSocket()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());

    // 처음 연결 시, 소켓과 클라이언트 정보를 매핑
    if (!fileSocketMap.contains(socket)) {
        protocol->receiveProtocol(socket);
        int id = protocol->packetData()->type();
        fileSocketMap.insert(socket, id);
        disconnect(socket, SIGNAL(readyRead()), this, SLOT(firstFileSocket()));
    }

    // MODALITY 소켓에만 리시브파일 연결
    if (protocol->packetData()->type() == MODALITY)
        connect(socket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
}

void SubServer::receiveControl()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    protocol->receiveProtocol(socket);

    if (!controlSocketMap.contains(socket)) {
        controlSocketMap.insert(socket, protocol->packetData()->type());
    }

    QString event = protocol->packetData()->event();
    QString msg = protocol->packetData()->msg();
    QString client = controlSocketMap.value(socket) ? "CT" : "촬영SW";      // controlSocketMap.value(socket) -> 0: 촬영SW, 1: 영상장비

    int receiver;
    if (controlSocketMap.value(socket)) {
        receiver = SW;
    } else {
        receiver = MODALITY;
    }

    if (event == "NEW") {
        ui->logEdit->append((QString("%1가 연결되었습니다.")).arg(client));
    } else if (event == "CTL") {
        if (receiver == MODALITY) {
            currentPID = msg.split("|")[0];
            currentType = msg.split("|")[1];
            msg = currentType;
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

        int command = protocol->packetData()->type();
        switch (command) {
        case RESET:
//            protocol->sendProtocol(controlSocketMap.key(receiver), "CTL", RESET, msg);
            ui->logEdit->append((QString("%1가 장비 초기화 명령을 보냈습니다.")).arg(client));
            break;
        case READY:
//            protocol->sendProtocol(controlSocketMap.key(receiver), "CTL", READY, msg);
            ui->logEdit->append((QString("%1가 %2 촬영준비 명령을 보냈습니다.")).arg(client, msg));
            break;
        case START:
//            protocol->sendProtocol(controlSocketMap.key(receiver), "CTL", START, msg);
            ui->logEdit->append((QString("%1가 %2 촬영시작 명령을 보냈습니다.")).arg(client, msg));
            break;
        case STOP:
//            protocol->sendProtocol(controlSocketMap.key(receiver), "CTL", STOP, msg);
            ui->logEdit->append((QString("%1가 %2 촬영종료 명령을 보냈습니다.")).arg(client, msg));
            break;
        }
    }
}

void SubServer::receiveFile()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    QByteArray tempArray;
    tempArray = socket->readAll();
    fileSocketMap.key(SW)->write(tempArray);
    receiveData.append(tempArray);


    if (receiveData.size() >= frameSize) {
        QDir dir(QString("receive/%1/%2/%3").arg(QDate::currentDate().toString("yyyyMMdd"), currentPID, currentType));
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
        file.write(receiveData.first(frameSize));
        file.close();
        receiveData.remove(0, frameSize);
        count++;
        qDebug("%d", count);
        if (count == countMax) {
            qDebug() << QString("%1 Frame Data Send End!").arg(currentType);
            count = 0;
            receiveData.clear();
            return;
        }
    }

}
