#include "mainnetworkmanager.h"

// 환자 준비 : IPR
// 저장 및 전송 : ISV

MainNetworkManager::MainNetworkManager(QObject *parent)
    : QObject{parent}
{
    mainSocket = new QTcpSocket(this);
    fileSocket = new QTcpSocket(this);

    connection("127.0.0.1", 8000);
}

MainNetworkManager::~MainNetworkManager()
{

}

void MainNetworkManager::connection(QString address, int port)
{
    mainSocket->connectToHost(address, port);
    if (mainSocket->waitForConnected()) {
        connect(mainSocket, SIGNAL(readyRead()), this, SLOT(receivePacket()));
    } else {
        // 연결 실패 예외처리 구현
    }

    fileSocket->connectToHost(address, port+1);
    if (fileSocket->waitForConnected()) {
        connect(fileSocket, SIGNAL(readyRead()), this, SLOT(recevieFile()));
        connect(fileSocket, SIGNAL(bytesWritten(qint64)), SLOT(goOnSend(qint64)));
    } else {
        // 연결 실패  예외처리 구현
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

    if (event == "VSR") {
        QStringList dataList;
        dataList << pid << data.split("|")[0] << data.split("|")[1];
        emit sendWaitPatient(dataList);
    }
}
