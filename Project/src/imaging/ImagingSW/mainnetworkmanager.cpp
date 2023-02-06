#include "mainnetworkmanager.h"

#include <QMessageBox>
#include <QDate>
#include <QPushButton>

// 환자 준비 : IPR<CR>PID<CR>"NULL"
// 환자 준비 후 정보 리시브 : IPR<CR>PID<CR>"name|sex|birth"
// 저장 및 전송 : ISV<CR>PID<CR>
// 촬영의뢰 : SRQ<CR>PID<CR>"name|type"

MainNetworkManager::MainNetworkManager(QObject *parent)
    : QObject{parent}
{
    mainSocket = new QTcpSocket(this);
    fileSocket = new QTcpSocket(this);

    disconnectBox = new QMessageBox;
    disconnectBox->setWindowTitle("Disconnect");
    disconnectBox->setText("서버와 연결이 끊어졌습니다. 재접속 하시겠습니까?");
    disconnectBox->addButton(QMessageBox::Retry);

//    mainConnection("192.168.0.39", 8000);
//    fileConnection("192.168.0.39", 8001);
}

MainNetworkManager::~MainNetworkManager()
{
    mainSocket->close();
    fileSocket->close();
    delete mainSocket;
    delete fileSocket;
}

void MainNetworkManager::mainConnection(QString address, int port)
{
    mainSocket->connectToHost(address, port);
    if (mainSocket->waitForConnected()) {
        connect(mainSocket, SIGNAL(readyRead()), this, SLOT(receivePacket()));
        connect(mainSocket, SIGNAL(disconnected()), this, SLOT(mainDisconnection()));
        sendPacket(mainSocket, "CNT", "IMG", "NULL");
    } else {
        mainDisconnection();
    }
}

void MainNetworkManager::fileConnection(QString address, int port)
{
    fileSocket->connectToHost(address, port+1);
    if (fileSocket->waitForConnected()) {
        connect(mainSocket, SIGNAL(disconnected()), this, SLOT(fileDisconnection()));
        sendPacket(fileSocket, "CNT", "IMG", "NULL");
    } else {
        fileDisconnection();
    }
}

void MainNetworkManager::mainDisconnection()
{
    if (disconnectBox->exec() == QMessageBox::Retry) {
        mainConnection("192.168.0.39", 8000);
    }
}

void MainNetworkManager::fileDisconnection()
{
    if (disconnectBox->exec() == QMessageBox::Retry) {
        fileConnection("192.168.0.39", 8001);
    }
}

void MainNetworkManager::sendPacket(QTcpSocket* socket, QString event, QString pid, QString data)
{
    QString sendData = event + "<CR>" + pid + "<CR>" + data;
    QByteArray sendArray = sendData.toStdString().c_str();
    socket->write(sendArray);
}

QStringList MainNetworkManager::packetParser(QByteArray receiveArray)
{
    QString receiveData = QString(receiveArray);

    QStringList dataList;
    dataList << receiveData.split("<CR>")[0] << receiveData.split("<CR>")[1] << receiveData.split("<CR>")[2];

    return dataList;
}

void MainNetworkManager::receivePacket()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    QStringList packetData = packetParser(socket->readAll());
    QString event = packetData[0];
    QString pid = packetData[1];
    QString data = packetData[2];

    QStringList dataList;
    if (event == "WTR") {       // WRT : 대기목록 리시브
        emit sendWaitList(pid.toInt(), data);
    } else if (event == "SRQ") {   // SRQ : 촬영의뢰 요청이 들어온 경우
        dataList << pid << data.split("|")[0] << data.split("|")[1];        // pid -> name -> type
        emit sendWaitPatient(dataList);
    } else if (event == "IPR") {    // IPR : 환자 정보 리시브
        dataList << pid << data.split("|")[0] << data.split("|")[1] << data.split("|")[2];        // pid -> name -> sex -> birth
        emit sendPatientInfo(dataList);
    }
}

void MainNetworkManager::requestPatientInfo(QString pid)
{
    sendPacket(mainSocket, "IPR", pid, "NULL");     // 서버로 환자정보 요청
}

void MainNetworkManager::endImagingProcess(QString pid, QString type)
{
    sendPacket(mainSocket, "ISV", pid, type);     // 촬영 종료 안내 패킷
}

void MainNetworkManager::sendFile(QString data)     // data = pid|shoot_type
{
    QObject::connect(fileSocket, SIGNAL(bytesWritten(qint64)), SLOT(goOnSend(qint64)));
    QString pid = data.split("|")[0];
    QString type = data.split("|")[1];

    byteToWrite = 0;
    totalSize = 0;
    outBlock.clear();

    QString imgName = pid + "_" + type + ".bmp";
    QString fileName = QString("recon/%1/%2").arg(QDate::currentDate().toString("yyyyMMdd"), imgName);
    qDebug() << fileName;
    if(fileName.length()) {
        file = new QFile(fileName);
        file->open(QFile::ReadOnly);

        qDebug() << QString("file %1 is opened").arg(fileName);

        byteToWrite = totalSize = file->size(); // Data remained yet

        QDataStream out(&outBlock, QIODevice::WriteOnly);
        out << qint64(0) << qint64(0) << pid << type;
        qDebug() << pid;
        qDebug() << type;

        totalSize += outBlock.size();
        byteToWrite += outBlock.size();

        out.device()->seek(0);
        out << totalSize << qint64(outBlock.size());

        fileSocket->write(outBlock); // Send the read file to the socket
    }
}

void MainNetworkManager::goOnSend(qint64 numBytes)
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());

    if (byteToWrite - numBytes >= 0) {
        byteToWrite -= numBytes; // Remaining data size
        outBlock = file->read(numBytes);
    } else {
        outBlock = file ->read(byteToWrite);
        byteToWrite = 0;
    }

    socket->write(outBlock);

    if (byteToWrite == 0) { // Send completed
        qDebug("File sending completed!");
        disconnect(fileSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
    }
}
