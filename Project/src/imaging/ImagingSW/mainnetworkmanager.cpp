/*
 * 프로그램명 : ImagingSW
 * 파일명 : mainnetworkmanager.cpp
 * 설명 : 메인 서버와 통신 구현
 * 작성자 : 안다미로
 * 최종 수정일 : 2023.02.13
 */


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

}

MainNetworkManager::~MainNetworkManager()
{
    mainSocket->deleteLater();
    fileSocket->deleteLater();
}

void MainNetworkManager::connectServer(QString address, int port)
{
    mainSocket = new QTcpSocket(this);
    fileSocket = new QTcpSocket(this);

    mainSocket->connectToHost(address, port);
    fileSocket->connectToHost(address, port+1);

    if (mainSocket->waitForConnected(1000) && fileSocket->waitForConnected(1000)) {
        connect(mainSocket, SIGNAL(readyRead()), this, SLOT(receivePacket()));
        connect(mainSocket, SIGNAL(disconnected()), this, SLOT(disconnectServer()));

        sendPacket(mainSocket, "SEN", "CNT", "IMG", "NULL");
        sendPacket(fileSocket, "SEN", "CNT", "IMG", "NULL");

        emit connectionStatusChanged(true);
    } else {
        QMessageBox disconnectBox(QMessageBox::Warning, "ERROR",
                                  "서버와 연결되지 않았습니다.",
                                  QMessageBox::Ok);
        disconnectBox.exec();
        return;
    }
}

void MainNetworkManager::disconnectServer()
{
    mainSocket->close();
    fileSocket->close();

    mainSocket->deleteLater();
    fileSocket->deleteLater();

    emit connectionStatusChanged(false);

    QMessageBox disconnectBox(QMessageBox::Warning, "ERROR",
                              "서버와 연결이 끊어졌습니다. 재접속 해주세요.",
                              QMessageBox::Ok);
    disconnectBox.exec();
    return;
}

void MainNetworkManager::sendPacket(QTcpSocket* socket, QString header, QString event, QString pid, QString data)
{
    if (socket == nullptr) {
        emit connectionStatusChanged(false);

        QMessageBox disconnectBox(QMessageBox::Warning, "ERROR",
                                  "서버와 연결이 끊어졌습니다. 재접속 해주세요.",
                                  QMessageBox::Ok);
        disconnectBox.exec();
        return;
    }

    QString sendData = header + "^" + event + "<CR>" + pid + "<CR>" + data;
    QByteArray sendArray = sendData.toStdString().c_str();
    socket->write(sendArray);
}

QStringList MainNetworkManager::packetParser(QByteArray receiveArray)
{
    QString receiveData = QString(receiveArray);
    QString header = (receiveData.split("<CR>")[0]).split("^")[0];
    QString event = (receiveData.split("<CR>")[0]).split("^")[1];

    QStringList dataList;
    dataList << header << event << receiveData.split("<CR>")[1] << receiveData.split("<CR>")[2];

    return dataList;
}

void MainNetworkManager::receivePacket()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    QStringList packetData = packetParser(socket->readAll());
    QString header = packetData[0];
    QString event = packetData[1];
    QString pid = packetData[2];
    QString data = packetData[3];

    QStringList dataList;

    if (header == "ACK") {
        if (event == "WTR") {       // WRT : 기존 대기목록 리시브
            emit sendWaitList(pid.toInt(), data);
        } else if (event == "SRQ") {   // SRQ : 촬영의뢰 요청이 들어온 경우
            dataList << pid << data.split("|")[0] << data.split("|")[1];        // pid -> name -> type
            emit sendWaitPatient(dataList);
        } else if (event == "IPR") {    // IPR : 환자 정보 리시브
            dataList << pid << data.split("|")[0] << data.split("|")[1] << data.split("|")[2];        // pid -> name -> sex -> birth
            emit sendPatientInfo(dataList);
        }
    }
}

void MainNetworkManager::requestPatientInfo(QString pid)
{
    sendPacket(mainSocket, "SEN", "IPR", pid, "NULL");     // 서버로 환자정보 요청
}

void MainNetworkManager::endImagingProcess(QString pid, QString type)
{
    sendPacket(mainSocket, "SEN", "ISV", pid, type);     // 촬영 종료 안내 패킷
}

void MainNetworkManager::sendFile(QString data)     // data = pid|shoot_type
{
    if (fileSocket == nullptr) {                    // 연결 체크
        QMessageBox disconnectBox(QMessageBox::Warning, "ERROR",
                                  "서버와 연결이 끊어졌습니다. 재접속 해주세요.",
                                  QMessageBox::Ok);
        disconnectBox.exec();
        emit connectionStatusChanged(false);
        return;
    }

    connect(fileSocket, SIGNAL(bytesWritten(qint64)), SLOT(sendingFile(qint64)));

    QString pid = data.split("|")[0];
    QString type = data.split("|")[1];

    remainBytes = 0;
    fileSize = 0;
    sendingArray.clear();

    QString imgName = pid + "_" + type;
    QString fileName = QString("image/recon/%1.jpg").arg(imgName);

    if(fileName.length()) {
        file = new QFile(fileName);
        if (!file->open(QFile::ReadOnly)) {
            return;
        }

        remainBytes = fileSize = file->size();      // Data remained yet

        QDataStream out(&sendingArray, QIODevice::WriteOnly);
        out << qint64(0) << qint64(0) << pid << type;

        fileSize += sendingArray.size();
        remainBytes += sendingArray.size();

        out.device()->seek(0);
        out << fileSize << qint64(sendingArray.size());

        fileSocket->write(sendingArray);            // Send the read file to the socket
    }
}

void MainNetworkManager::sendingFile(qint64 numBytes)
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());

    if (remainBytes - numBytes >= 0) {
        remainBytes -= numBytes;                // 남은 바이트 수 계산
        sendingArray = file->read(numBytes);
    } else {
        sendingArray = file ->read(remainBytes);
        remainBytes = 0;
    }

    socket->write(sendingArray);

    if (remainBytes == 0) { // 파일 전송 완료
        disconnect(fileSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(sendingFile(qint64)));
        QMessageBox disconnectBox(QMessageBox::NoIcon, "ERROR",
                                  "파일 전송이 완료되었습니다.",
                                  QMessageBox::Ok);
        disconnectBox.exec();
        delete file;
    }
}
