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

SubServer::SubServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubServer)
{
    ui->setupUi(this);

    protocol = new Protocol();

    // 장비제어 명령 서버 오픈
    controlServer = new QTcpServer();
    connect(controlServer, SIGNAL(newConnection()), this, SLOT(newClient()));
    if(!controlServer->listen(QHostAddress::Any, 8000)) {
        // 서버 listen 실패
    }

    // 이미지 서버 오픈
    fileServer = new QTcpServer();
    connect(fileServer, SIGNAL(newConnection()), this, SLOT(newFileCilent()));
    if(!fileServer->listen(QHostAddress::Any, 8001)) {
        // 파일 서버 listen 실패
    }

    progressDialog = new QProgressDialog(0);
    progressDialog->setAutoClose(true);
    progressDialog->reset();
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

void SubServer::newFileCilent()
{
    QTcpSocket *fileSocket = fileServer->nextPendingConnection();
    connect(fileSocket, SIGNAL(readyRead()), this, SLOT(recevieFile()));
    connect(fileSocket, SIGNAL(bytesWritten(qint64)), SLOT(goOnSend(qint64)));
}

void SubServer::receiveControl()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    protocol->receiveProtocol(socket);

    if (!controlSocketMap.contains(socket)) {
        controlSocketMap.insert(socket, protocol->packetData()->index());    // pid() : SW(100) or MODALITY(101)
    }

    QString event = protocol->packetData()->event();
    QString client = controlSocketMap.value(socket) ? "영상장비" : "촬영SW";

    int receiver;
    if (controlSocketMap.value(socket)) receiver = SW;
    else receiver = MODALITY;

    if (event == "NEW") {
        ui->logEdit->append((QString("%1가 연결되었습니다.")).arg(client));
        sendFile();
    } else if (event == "CTL") {
        int command = protocol->packetData()->index();
        switch (command) {
        case RESET:
            //protocol->sendProtocol(controlSocketMap.key(receiver), "CTL", RESET, "");
            ui->logEdit->append((QString("%1가 장비 초기화 명령을 보냈습니다.")).arg(client));
            break;
        case READY:
            //protocol->sendProtocol(controlSocketMap.key(receiver), "CTL", READY, "");
            ui->logEdit->append((QString("%1가 촬영준비 명령을 보냈습니다.")).arg(client));
            break;
        case START:
            //protocol->sendProtocol(controlSocketMap.key(receiver), "CTL", START, "");
            ui->logEdit->append((QString("%1가 촬영시작 명령을 보냈습니다.")).arg(client));
            break;
        case STOP:
            //protocol->sendProtocol(controlSocketMap.key(receiver), "CTL", STOP, "");
            ui->logEdit->append((QString("%1가 촬영종료 명령을 보냈습니다.")).arg(client));
            break;
        }
    }
}

void SubServer::recevieFile()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());

    // 처음 연결 시, 소켓과 클라이언트 정보를 매핑
    if (!fileSocketMap.contains(socket)) {
        int id = protocol->packetData()->index();
        protocol->receiveProtocol(socket);
        fileSocketMap.insert(socket, id);
        return;
    }

    // Beginning File Transfer
    if (byteReceived == 0) {        // First Time(Block) , var byteReceived is always zero
        progressDialog->reset();
        progressDialog->show();
        checkFileName = fileName;
        QDataStream in(socket);
        in.device()->seek(0);
        in >> totalSize >> byteReceived >> fileName >> fileSender;
        if(checkFileName == fileName) return;

        progressDialog->setMaximum(totalSize);
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

    progressDialog->setValue(byteReceived);
    if (byteReceived == totalSize) {        // file sending is done
        qDebug() << QString("%1 receive completed").arg(fileName);
        inBlock.clear();
        byteReceived = 0;
        totalSize = 0;
        progressDialog->reset();
        progressDialog->hide();
        file->close();
        delete file;
    }
}

void SubServer::goOnSend(qint64 numBytes)
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    byteToWrite -= numBytes; // Remaining data size
    outBlock = file->read(qMin(byteToWrite, numBytes));
    socket->write(outBlock);

    progressDialog->setMaximum(totalSize);
    progressDialog->setValue(totalSize-byteToWrite);

    if (byteToWrite == 0) { // Send completed
        qDebug("File sending completed!");
        progressDialog->reset();
    }
}

void SubServer::sendFile()
{
    loadSize = 0;
    byteToWrite = 0;
    totalSize = 0;
    outBlock.clear();

    QString filename = QFileDialog::getOpenFileName(this);
    if(filename.length()) {
        file = new QFile(filename);
        file->open(QFile::ReadOnly);

        qDebug() << QString("file %1 is opened").arg(filename);
        progressDialog->setValue(0); // first time, any data send

        byteToWrite = totalSize = file->size(); // Data remained yet
        loadSize = 1024; // Size of data per a block
        qDebug("%d", __LINE__);
        QDataStream out(&outBlock, QIODevice::WriteOnly);
        out << qint64(0) << qint64(0) << filename << "SERVER";

        totalSize += outBlock.size();
        byteToWrite += outBlock.size();

        out.device()->seek(0);
        out << totalSize << qint64(outBlock.size());

        fileSocketMap.key(SW)->write(outBlock); // Send the read file to the socket

        progressDialog->setMaximum(totalSize);
        progressDialog->setValue(totalSize-byteToWrite);
        progressDialog->show();
    }
    qDebug() << QString("Sending file %1").arg(filename);
}



void SubServer::on_pushButton_clicked()
{
    sendFile();
}

