#include "networkmanager.h"

#include <QBoxLayout>
#include <QDataStream>
#include <QTcpSocket>

NetworkManager::NetworkManager(QObject *parent)
    : QObject{parent}
{
    socket = new QTcpSocket(this);
    fd_flag = connectToHost("127.0.0.1"); // localhost
    connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
    qDebug("%d", __LINE__);

    if(!fd_flag)
        qDebug()<<("Socket connect fail\n");
    else{
        qDebug("%d", __LINE__);
        qDebug()<<("Socket connect\n");
        QString connectData = "CNT<CR>PMS<CR>";

        qDebug("%d", __LINE__);
        QByteArray sendTest = connectData.toStdString().c_str();
        qDebug("%d", __LINE__);
        socket->write(sendTest);
        qDebug("%d", __LINE__);
    }
}

bool NetworkManager::connectToHost(QString host)
{
    socket->connectToHost(host, 8000);
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
    qDebug("%d", __LINE__);
    socket = static_cast<QTcpSocket*>(sender());
    //buffer = buffers.value(socket);
    qDebug("%d", __LINE__);
    //buffer->append(socket->readAll());
    QByteArray array = socket->readAll();
    qDebug("%d", __LINE__);
    //    saveData = QString(buffer->data());
    saveData = QString(array);
    qDebug("%d", __LINE__);

    if(saveData.contains("<CR", Qt::CaseInsensitive) == true)
    {
        //어떤 이벤트인지에 따라 불러올 함수 써주기(각각 이벤트에 대한 함수 만들고 if-else문 타도록 만들자)
        QString event = saveData.split("<CR>")[0];
        QString id = saveData.split("<CR>")[1];
        QString data = saveData.split("<CR>")[2];
        qDebug("%d", __LINE__);
        qDebug() << "event: " << event;

        if(event == "PID")
        {
            sendedPID = id;
            qDebug() << "sendedPID: " << id;
            emit sendNewPID(sendedPID); //enrollment 클래스로 emit
        }
        else if(event == "PSE")
        {
            qDebug() << "PSE data: " << data;
            emit sendSearchResult(id, data);    //patientInfoManager 클래스와 medicalRecordManager 클래스 두 곳으로 모두 보내줘야 함
        }
        else if(event == "SRQ")
        {
            qDebug()<<"SRQ event Received: " << saveData;
            emit sendSRQRequest(saveData);
        }
        else if(event == "VTS")
        {
            qDebug()<<"VTS event Received: " << saveData;
            emit sendVTSRequest(saveData);
        }
        else if(event == "ISV")
        {
            qDebug()<<"ISV event Received: " << saveData;
            emit sendISVevent(saveData);
        }
        else if(event == "VTF")
        {
            qDebug()<<"VTF event Received: " << saveData;
            emit sendVTFevent(saveData);
        }




        //    buffer->clear(); //버퍼 비워주기
    }
}

//void NetworkManager::newConnection()
//{
//    while (server->hasPendingConnections())
//    {
//        QTcpSocket *socket = server->nextPendingConnection();
//        connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
//        connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
//        QByteArray *buffer = new QByteArray();
//        qint32 *s = new qint32(0);
//        buffers.insert(socket, buffer);
//        sizes.insert(socket, s);
//    }
//}

//void NetworkManager::disconnected()
//{
//    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
//    QByteArray *buffer = buffers.value(socket);
//    qint32 *s = sizes.value(socket);
//    socket->deleteLater();
//    delete buffer;
//    delete s;
//}
