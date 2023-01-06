#include "controlclient.h"
#include <QTcpSocket>

SubClient::SubClient(QObject *parent)
    : QObject{parent}
{
    socket = new QTcpSocket(this);

    socket->connectToHost("192.168.0.46", 8000);
    if (socket->waitForConnected()) {
        qDebug("Connection Success!");
        connect(socket, SIGNAL(readyRead()), SLOT(receiveData()));

        QByteArray dataArray;
        QDataStream out(&dataArray, QIODevice::WriteOnly);
        out.device()->seek(0);
        out.writeRawData("Test1!!!!", 1020);
        socket->write(dataArray);     // Send data to server
        socket->flush();
        while(socket->waitForBytesWritten());
    }
}

SubClient::~SubClient()
{
    socket->close();
}
