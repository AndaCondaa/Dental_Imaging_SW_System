/*
 * 프로그램명 : SubServer
 * 파일명 : subserver.cpp
 * 설명 : 영상 검사실용 미니 서버 (촬영SW와 Modality의 연결)
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.13
 */

#include "subserver.h"
#include "ui_subserver.h"

#include "protocol.h"
#include "packetdata.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QDir>
#include <QMessageBox>

SubServer::SubServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubServer)
{
    ui->setupUi(this);

    protocol = new Protocol();

    // 장비제어 명령 서버 오픈
    controlServer = new QTcpServer();
    connect(controlServer, SIGNAL(newConnection()), this, SLOT(newClient()));
    if(controlServer->listen(QHostAddress::Any, 8002)) {
        ui->controlLabel->setText(QString("[PORT %1]  LISTEN...").arg(controlServer->serverPort()));
    } else {
        QMessageBox errorBox(QMessageBox::Critical, "SERVER ERROR",
                             QString("Listen Fail..."),
                             QMessageBox::Ok, this, Qt::Dialog);
        errorBox.exec();
        qApp->quit();
    }

    // 파일 서버 오픈
    fileServer = new QTcpServer();
    connect(fileServer, SIGNAL(newConnection()), this, SLOT(newFileClient()));
    if(fileServer->listen(QHostAddress::Any, 8003)) {
        ui->fileLabel->setText(QString("[PORT %1]  LISTEN...").arg(fileServer->serverPort()));
    } else {
        QMessageBox errorBox(QMessageBox::Critical, "SERVER ERROR",
                             QString("Listen Fail..."),
                             QMessageBox::Ok, this, Qt::Dialog);
        errorBox.exec();
        qApp->quit();
    }
}

SubServer::~SubServer()
{
    delete ui;

    controlServer->close();
    fileServer->close();

    delete controlServer;
    delete fileServer;
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
        connect(socket, &QTcpSocket::disconnected, this, [=](){
            fileSocketMap.remove(socket);
            socket->deleteLater();
        });
    }

    // MODALITY 소켓에만 리시브파일 연결
    if (protocol->packetData()->type() == ConnectType::MODALITY)
        connect(socket, SIGNAL(readyRead()), this, SLOT(receiveFile()));
}

void SubServer::receiveControl()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    protocol->receiveProtocol(socket);

    QString event = protocol->packetData()->event();
    int controlType = protocol->packetData()->type();
    QString msg = protocol->packetData()->msg();
    QString ip = QString(socket->peerAddress().toString());
    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    int receiver = -1;

    if (!controlSocketMap.contains(socket)) {
        controlSocketMap.insert(socket, protocol->packetData()->type());
        connect(socket, &QTcpSocket::disconnected, this, [=](){
            ui->logEdit->append((QString("[%1] [%2] [%3]").arg(date, ip, "DISCONNECT")));
            controlSocketMap.remove(socket);
            socket->deleteLater();
        });
    }

    if (controlSocketMap.value(socket) == ConnectType::SW) {
        receiver = ConnectType::MODALITY;
    } else if (controlSocketMap.value(socket) == ConnectType::MODALITY) {
        receiver = ConnectType::SW;
    }

    if (event == "NEW") {
        ui->logEdit->append(QString(QString("[%1] [%2] [%3]").arg(date, ip, event)));
    } else if (event == "CTL") {    
        if (controlSocketMap.key(receiver, nullptr) == nullptr) {
            protocol->sendProtocol(socket, "ERR", event, protocol->packetData()->type(), msg);
            return;
        }

        if (receiver == MODALITY) {
            currentPID = msg.split("|")[0];
            currentType = msg.split("|")[1];
            msg = currentType;
            if (currentType == "PANO") {
                countMax = 1750;
                frameSize = 1152 * 64 * 2;
            } else if (currentType == "CEPH") {
                countMax = 1250;
                frameSize = 48 * 2400 * 2;
            }
        }

        if (controlType == ControlType::START || controlType == ControlType::STOP) {
            count = 0;
            receiveData.clear();
        }

        ui->logEdit->append((QString("[%1] [%2] [%3_%4]").arg(date, ip, QString::number(controlType), msg)));

        if (controlType == ControlType::STOP) {
            protocol->sendProtocol(controlSocketMap.key(ConnectType::SW), "ACK", "CTL", controlType, msg);

            QTcpSocket *disconnectModality = new QTcpSocket(this);
            disconnectModality = fileSocketMap.key(ConnectType::MODALITY);
            disconnectModality->close();
            disconnectModality->deleteLater();
            controlSocketMap.remove(disconnectModality);
        } else {
            protocol->sendProtocol(controlSocketMap.key(ConnectType::SW), "ACK", "CTL", controlType, msg);
            protocol->sendProtocol(controlSocketMap.key(ConnectType::MODALITY), "ACK", "CTL", controlType, msg);
        }
    }
}

void SubServer::receiveFile()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    QByteArray tempArray;
    tempArray = socket->readAll();
    fileSocketMap.key(ConnectType::SW)->write(tempArray);
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
        if (count == countMax) {
            qDebug() << QString("%1 Frame Data Send End!").arg(currentType);
            count = 0;
            receiveData.clear();
            return;
        }
    }
}
