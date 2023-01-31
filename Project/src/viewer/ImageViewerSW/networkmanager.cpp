#include "networkmanager.h"

#include <QBoxLayout>
#include <QDataStream>
#include <QTcpSocket>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>

NetworkManager::NetworkManager(QObject *parent)
    : QObject{parent}
{
//    socket = new QTcpSocket(this);

//    fd_flag = connectToHost("192.168.0.39");
//    connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));

//    if(!fd_flag)
//        qDebug()<<("Socket connect fail\n");
//    else {
//        qDebug()<<("Socket connect success\n");
//        QString connectData = "CNT<CR>VEW<CR>";
//        QByteArray sendTest = connectData.toStdString().data();
//        socket->write(sendTest);
//    }

//    fileSocket = new QTcpSocket(this);
//    fileSocket->connectToHost("192.168.0.39", 8001);
//    connect(fileSocket, SIGNAL(readyRead()), this, SLOT(receiveFile()));

//    if(fileSocket->waitForConnected())
//        fileSocket->write("CNT<CR>VEW<CR>NULL");
//    else
//        qDebug()<<("FileServer connect failed\n");
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

void NetworkManager::receiveFile()
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());

    if (fileSocket != socket) {
        QByteArray arr = socket->readAll();
        QString id = QString(arr).split("<CR>")[1];

        if (id == "VEW") {  //근데 여기서는 굳이 소켓을 멤버변수로 설정하지는 않아도 될 것 같음. 소켓이 하나밖에 없어서..
            fileSocket = socket;
        }
        return;
    }

    if (byteReceived == 0) {                                    // First Time(Block) , var byteReceived is always zero
        checkFileName = fileName;                               // 다음 패킷부터 파일이름으로 구분하기 위해 첫 패킷에서 보낸 파일이름을 임시로 저장

        QDataStream in(fileSocket);
        in.device()->seek(0);
        in >> totalSize >> byteReceived >> fileName;
        if(checkFileName == fileName) return;

        QFileInfo info(fileName);
        currentPID = info.fileName();

        QDir dir(QString("./Image/%1").arg(currentPID.first(6)));   //ex.P00001
        if (!dir.exists())
            dir.mkpath(".");

        QString currentFileName = dir.path() + "/" +info.fileName();

        file = new QFile(currentFileName);
        file->open(QFile::WriteOnly);
    }

    else {
        if(checkFileName == fileName) return;
        inBlock = fileSocket->readAll();

        byteReceived += inBlock.size();
        file->write(inBlock);
        file->flush();
    }

    if (byteReceived == totalSize) {        // file sending is done
        qDebug() << QString("%1 receive completed").arg(fileName);
        inBlock.clear();
        byteReceived = 0;
        totalSize = 0;
        file->close();
        delete file;
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
    socket = static_cast<QTcpSocket*>(sender());
    QByteArray array = socket->readAll();
    saveData = QString(array);
    qDebug() << "데이터 : " <<saveData;

    if(saveData.contains("<CR", Qt::CaseInsensitive) == true)
    {
        //어떤 이벤트인지에 따라 불러올 함수 써주기(각각 이벤트에 대한 함수 만들고 if-else문 타도록 만들자)
        QString event = saveData.split("<CR>")[0];
        QString id = saveData.split("<CR>")[1];
        QString data = saveData.split("<CR>")[2];

        //AWL : 대기 환자 추가
        if(event == "AWL")
        {
            emit sendWaitingList(id, data);
        }

        //대기 리스트에서 선택된 환자 정보 추가
        else if(event == "VTS")
        {
            emit sendSelectPatient(id, data);
        }

        //PMS에서 촬영을 시작하는 환자의 진행 상황 추가
        else if(event == "SRQ")
        {
            emit sendPMSCameraPatient(id, data);
        }

        else if(event == "VLG")
        {
            emit sendLogInCheck(data);
        }

        else if(event == "ISV")
        {
            emit sendPhotoEnd(id);
        }
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
