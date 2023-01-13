#include "mainnetworkmanager.h"

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
        connect(mainSocket, SIGNAL(readyRead()), SLOT(receiveControl()));
    } else {
        // 연결 실패 예외처리 구현
    }

    fileSocket->connectToHost(address, port+1);
    if (fileSocket->waitForConnected()) {
        connect(fileSocket, SIGNAL(readyRead()), SLOT(receiveFile()));
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

void MainNetworkManager::receivePacket()
{

}
