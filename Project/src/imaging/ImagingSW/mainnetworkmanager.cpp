#include "mainnetworkmanager.h"

#include <QFileDialog>

// 환자 준비 : IPR
// 저장 및 전송 : ISV

MainNetworkManager::MainNetworkManager(QObject *parent)
    : QObject{parent}
{
    mainSocket = new QTcpSocket(this);
    fileSocket = new QTcpSocket(this);

    connection("192.168.0.57", 8001);

    progressDialog = new QProgressDialog(0);
    progressDialog->setAutoClose(true);
    progressDialog->reset();
}

MainNetworkManager::~MainNetworkManager()
{

}

void MainNetworkManager::connection(QString address, int port)
{
//    mainSocket->connectToHost(address, port);
//    if (mainSocket->waitForConnected()) {
//        connect(mainSocket, SIGNAL(readyRead()), this, SLOT(receivePacket()));
//    } else {
//        // 연결 실패 예외처리 구현
//    }

//    fileSocket->connectToHost(address, port+1);
//    if (fileSocket->waitForConnected()) {
//        //connect(fileSocket, SIGNAL(readyRead()), this, SLOT(recevieFile()));
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
    dataList << receiveData.split("<CR>")[0] << receiveData.split("<CR>")[1] << receiveData.split("<CR>")[3];

    return dataList;
}

void MainNetworkManager::receivePacket()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    QStringList dataList = packetParser(socket->readAll());
    QString event = dataList[0];
    QString pid = dataList[1];
    QString data = dataList[2];

    if (event == "VSR") {   // VSR : 촬영의뢰 요청이 들어온 경우
        QStringList dataList;
        dataList << pid << data.split("|")[0] << data.split("|")[1];
        emit sendWaitPatient(dataList);
    }
}

void MainNetworkManager::requestPatientInfo(QString pid)
{
    sendPacket(mainSocket, "IPR", pid, "NULL");     // 서버로 환자정보 요청
}

void MainNetworkManager::goOnSend(qint64 numBytes)
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

void MainNetworkManager::sendFile()
{
    loadSize = 0;
    byteToWrite = 0;
    totalSize = 0;
    outBlock.clear();

    QString filename = QFileDialog::getOpenFileName();
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

        fileSocket->write(outBlock); // Send the read file to the socket

        progressDialog->setMaximum(totalSize);
        progressDialog->setValue(totalSize-byteToWrite);
        progressDialog->show();
    }
    qDebug() << QString("Sending file %1").arg(filename);
}
