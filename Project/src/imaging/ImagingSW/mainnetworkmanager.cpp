#include "mainnetworkmanager.h"

#include <QFileDialog>

// 환자 준비 : IPR<CR>PID<CR>"NULL"
// 환자 준비 후 정보 리시브 : IPR<CR>PID<CR>"name|sex|birth"
// 저장 및 전송 : ISV<CR>PID<CR>
// 촬영의뢰 : SRQ<CR>PID<CR>"name|type"

MainNetworkManager::MainNetworkManager(QObject *parent)
    : QObject{parent}
{
    mainSocket = new QTcpSocket(this);
    fileSocket = new QTcpSocket(this);

    connection("192.168.0.39", 8000);
}

MainNetworkManager::~MainNetworkManager()
{

}

void MainNetworkManager::connection(QString address, int port)
{
    //    mainSocket->connectToHost(address, port);
    //    if (mainSocket->waitForConnected()) {
    //        connect(mainSocket, SIGNAL(readyRead()), this, SLOT(receivePacket()));
    //        sendPacket(mainSocket, "CNT", "IMG", "NULL");
    //    } else {
    //        // 연결 실패 예외처리 구현
    //    }

    //    fileSocket->connectToHost(address, port+1);
    //    if (fileSocket->waitForConnected()) {
    //        sendPacket(fileSocket, "CNT", "IMG", "NULL");
    //        connect(fileSocket, SIGNAL(bytesWritten(qint64)), SLOT(goOnSend(qint64)));
    //    } else {
    //        // 연결 실패  예외처리 구현
    //    }
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
    if (event == "SRQ") {   // SRQ : 촬영의뢰 요청이 들어온 경우
        dataList << pid << data.split("|")[0] << data.split("|")[1];        // pid -> name -> type
        emit sendWaitPatient(dataList);
    } else if (event == "IPR") {
        dataList << pid << data.split("|")[0] << data.split("|")[1] << data.split("|")[2];        // pid -> name -> sex -> birth
        emit sendPatientInfo(dataList);
    }
}

void MainNetworkManager::requestPatientInfo(QString pid)
{
    sendPacket(mainSocket, "IPR", pid, "NULL");     // 서버로 환자정보 요청
}

void MainNetworkManager::endImagingProcess(QString pid)
{
    sendPacket(mainSocket, "ISV", pid, "NULL");     // 촬영 종료 안내 패킷
}

void MainNetworkManager::sendFile(QString data)     // data = pid|shoot_type
{
    QString pid = data.split("|")[0];
    QString type = data.split("|")[1];

    loadSize = 0;
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
        loadSize = 1024; // Size of data per a block

        QDataStream out(&outBlock, QIODevice::WriteOnly);
        out << qint64(0) << qint64(0) << pid << type << fileName;

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
    byteToWrite -= numBytes; // Remaining data size
    outBlock = file->read(qMin(byteToWrite, numBytes));
    socket->write(outBlock);

    if (byteToWrite == 0) { // Send completed
        qDebug("File sending completed!");
    }
}
