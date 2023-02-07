#include "mainnetworkmanager.h"

#include <QApplication>
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

//    connectSever("10.222.0.153", 8000);
}

MainNetworkManager::~MainNetworkManager()
{
//    mainSocket->close();
//    fileSocket->close();
//    delete mainSocket;
//    delete fileSocket;
}

void MainNetworkManager::connectSever(QString address, int port)
{
    mainSocket->connectToHost(address, port);
    if (mainSocket->waitForConnected()) {
        connect(mainSocket, SIGNAL(readyRead()), this, SLOT(receivePacket()));
        connect(mainSocket, SIGNAL(disconnected()), this, SLOT(disconnectServer()));
        sendPacket(mainSocket, "CNT", "IMG", "NULL");
    } else {
        qDebug("Connection FAIL! (MAIN)");
    }

    fileSocket->connectToHost(address, port++);
    if (fileSocket->waitForConnected()) {
        connect(fileSocket, SIGNAL(disconnected()), this, SLOT(disconnectServer()));
        sendPacket(fileSocket, "CNT", "IMG", "NULL");
    } else {
        qDebug("Connection FAIL! (FILE)");
    }
}

void MainNetworkManager::disconnectServer()
{
    disconnect(mainSocket, SIGNAL(readyRead()), this, SLOT(receivePacket()));
    disconnect(mainSocket, SIGNAL(disconnected()), this, SLOT(disconnectSever()));
    disconnect(fileSocket, SIGNAL(disconnected()), this, SLOT(disconnectSever()));

    mainSocket->close();
    fileSocket->close();
    delete mainSocket;
    delete fileSocket;

    disconnectBox = new QMessageBox;
    disconnectBox->setWindowTitle("Disconnect");
    disconnectBox->setText("서버와 연결이 끊어졌습니다.");
    disconnectBox->addButton(QMessageBox::Close);

    if (disconnectBox->exec() == QMessageBox::Close) {
        qApp->quit();
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

    QString imgName = pid + "_" + type;
    QString fileName = QString("image/recon/%1.bmp").arg(imgName);
    qDebug() << fileName;
    if(fileName.length()) {
        file = new QFile(fileName);
        file->open(QFile::ReadOnly);

        byteToWrite = totalSize = file->size(); // Data remained yet

        QDataStream out(&outBlock, QIODevice::WriteOnly);
        out << qint64(0) << qint64(0) << pid << type;

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
