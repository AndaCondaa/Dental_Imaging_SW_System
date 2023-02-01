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
    QString client = controlSocketMap.value(socket) ? "영상장비" : "촬영SW";      // controlSocketMap.value(socket) -> 0: 촬영SW, 1: 영상장비

    int receiver;
    if (controlSocketMap.value(socket)) receiver = SW;
    else receiver = MODALITY;

    if (event == "NEW") {
        ui->logEdit->append((QString("%1가 연결되었습니다.")).arg(client));
    } else if (event == "CTL") {
        int command = protocol->packetData()->type();
        currentPID = msg.split("|")[0];
        currentType = msg.split("|")[1];
        switch (command) {
        case RESET:
            protocol->sendProtocol(controlSocketMap.key(receiver), "CTL", RESET, msg);
            ui->logEdit->append((QString("%1가 장비 초기화 명령을 보냈습니다.")).arg(client));
            break;
        case READY:
            protocol->sendProtocol(controlSocketMap.key(receiver), "CTL", READY, msg);
            ui->logEdit->append((QString("%1가 %2의 %3 촬영준비 명령을 보냈습니다.")).arg(client, currentPID, currentType));
            break;
        case START:
            protocol->sendProtocol(controlSocketMap.key(receiver), "CTL", START, msg);
            ui->logEdit->append((QString("%1가 %2의 %3 촬영시작 명령을 보냈습니다.")).arg(client, currentPID, currentType));
            break;
        case STOP:
            protocol->sendProtocol(controlSocketMap.key(receiver), "CTL", STOP, msg);
            ui->logEdit->append((QString("%1가 %2의 %3 촬영종료 명령을 보냈습니다.")).arg(client, currentPID, currentType));
            break;
        }
    }
}

void SubServer::receiveFile()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    QByteArray recvData = socket->readAll();



    if (QString(recvData).contains("<CR>")) {
        qDebug() << "CONTAIN";
        qDebug() << QString(recvData).split("<CR>")[1];
        qDebug() << QString(recvData).split("<CR>")[2];
    }

    totalData.append(recvData);

    qDebug("%d", totalData.size());
//    if (totalData.size() == 227865600) {
//        QFile file;
//        QString fileName;

//        QDir dir(QString("receive/%1/%2/").arg(QDate::currentDate().toString("yyyyMMdd"), currentPID));
//        if (!dir.exists())
//            dir.mkpath(".");

//        int count = 0;
//        for (int i = 10; i < 999; i++) {
//            if (i >= 100)
//                fileName = dir.path() + "/" + QString("0%1.raw").arg(i);
//            else
//                fileName = dir.path() + "/" + QString("00%1.raw").arg(i);

//            file.setFileName(fileName);
//            file.open(QIODevice::WriteOnly);

//            file.write(totalData.mid(count*48*2400*2, 48*2400*2));
//            qDebug("%d개 저장 완료", count);

//            file.close();
//            count++;
//        }
//        qDebug("DONE!!!! (line: %d)", __LINE__);
////        sendFile();
//    }
}

void SubServer::sendFile()
{
    fileSocketMap.key(SW)->write(totalData);
    fileSocketMap.key(SW)->flush();
    qDebug("TOTAL SIZE : %d", totalData.size());
    totalData.clear();

}
