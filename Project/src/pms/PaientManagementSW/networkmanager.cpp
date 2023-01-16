#include "networkmanager.h"

#include <QBoxLayout>
#include <QDataStream>
#include <QTcpSocket>

NetworkManager::NetworkManager(QObject *parent)
    : QObject{parent}
{
    socket = new QTcpSocket();
    fd_flag = connectToHost("127.0.0.1"); // localhost
    if(!fd_flag)
        qDebug()<<("Socket connect fail\n");

}

bool NetworkManager::connectToHost(QString host)
{
    socket->connectToHost(host, 8001);
    return socket->waitForConnected();
}

//QByteArray IntToArray(qint32 source)
//{
//    QByteArray temp;
//    QDataStream data(&temp, QIODevice::ReadWrite);
//    data << source;
//    return temp;
//}


bool NetworkManager::writeData(QByteArray data)
{
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        //socket->write(IntToArray(data.size()));
        socket->write(data); // 데이터를 보내줌
        return socket->waitForBytesWritten();
    }
    else
    {
        return false;
    }
}

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

void NetworkManager::receiveData()
{

    socket = static_cast<QTcpSocket*>(sender());
    buffer = buffers.value(socket);

    buffer->append(socket->readAll());
    saveData = QString(buffer->data());


    //어떤 이벤트인지에 따라 불러올 함수 써주기(각각 이벤트에 대한 함수 만들고 if-else문 타도록 만들자)
    QString event = saveData.split("<CR>")[0];
    QString id = saveData.split("<CR>")[1];
    QString data = saveData.split("<CR>")[2];

    if(event == "PID")
    {
        sendedPID = id;
        qDebug() << "sendedPID: " << id;
        //emit sendNewPID(newPID); //enrollment 클래스로 emit
    }

    buffer->clear(); //버퍼 비워주기
}
