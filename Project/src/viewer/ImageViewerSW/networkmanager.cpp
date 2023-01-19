#include "networkmanager.h"

#include <QBoxLayout>
#include <QDataStream>
#include <QTcpSocket>

NetworkManager::NetworkManager(QObject *parent)
    : QObject{parent}
{
    socket = new QTcpSocket(this);

    fd_flag = connectToHost("192.168.0.57"); // localhost : 192.168.0.57
    connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));

    if(!fd_flag)
        qDebug()<<("Socket connect fail\n");

    else {
        qDebug()<<("Socket connect success\n");

        QString connectData = "CNT<CR>VEW<CR>";
        QByteArray sendTest = connectData.toStdString().data();
        socket->write(sendTest);
    }

}

bool NetworkManager::connectToHost(QString host)
{
    socket->connectToHost(host, 8000);
    return socket->waitForConnected();
}


bool NetworkManager::writeData(QByteArray data)
{
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->write(data); // 데이터를 보내줌
        return socket->waitForBytesWritten();
    }
    else
    {
        return false;
    }
}

//서버로 보내줄 데이터
void NetworkManager::newDataSended(QString newData)
{

    if(fd_flag)
    {
        QString sendData = newData; //MainServer의 textEdit에 띄울 정보
        send_flag = writeData(sendData.toStdString().c_str()); //writeData의 첫 번째 인자는 char *data와 같은 형식임
        if(!send_flag)
            qDebug() << "Socket send fail\n";
    }
}

//서버에서 받아올 데이터
void NetworkManager::receiveData()
{
    qDebug() << "데이터 : ";
    socket = static_cast<QTcpSocket*>(sender());
    QByteArray array = socket->readAll();
    saveData = QString(array);
    qDebug() << "데이터 : "<<saveData;

    if(saveData.contains("<CR", Qt::CaseInsensitive) == true)
    {
        //어떤 이벤트인지에 따라 불러올 함수 써주기(각각 이벤트에 대한 함수 만들고 if-else문 타도록 만들자)
        QString event = saveData.split("<CR>")[0];
        QString id = saveData.split("<CR>")[1];
        QString data = saveData.split("<CR>")[2];
        qDebug() << "event: " << event;

        //AWL : 대기 환자 추가
        if(event == "AWL")
        {
            qDebug() << "sendedId: " << id << ", 이름 : " << data;
            emit sendWaitingList(id, data);
        }

        //대기 리스트에서 선택된 환자 정보 추가
        else if(event == "VTS")
        {
            qDebug() << "sendedId: " << id << ", 데이터 : " << data;
            emit sendSelectPatient(id, data);
        }

        //PMS에서 촬영을 시작하는 환자의 진행 상황 추가
        else if(event == "SRQ")
        {
            qDebug() << "촬영 정보" << id << data;
            emit sendPMSCameraPatient(id, data);

        }

        else if(event == "VLG")
        {
            qDebug() << "로그인 정보" << id << data;
            emit sendLogInCheck(data);
        }

        else if(event == "ISV")
        {
            qDebug() << "촬영 완료 정보" << id << data;
            emit sendPhotoEnd(id);
        }
    }
}

void NetworkManager::newConnection()
{
    while (server->hasPendingConnections())
    {
        QTcpSocket *socket = server->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
        QByteArray *buffer = new QByteArray();
        qint32 *s = new qint32(0);
        buffers.insert(socket, buffer);
        sizes.insert(socket, s);
    }
}

void NetworkManager::disconnected()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket);
    qint32 *s = sizes.value(socket);
    socket->deleteLater();
    delete buffer;
    delete s;
}
