#include "protocol.h"

Protocol::Protocol(QTcpSocket *socket)
{
    memberSocket = new QTcpSocket();
    memberSocket = socket;
}

Protocol::~Protocol()
{
    memberSocket->close();
}

void Protocol::setSocket(QTcpSocket *socket)
{
    memberSocket = socket;
}

QTcpSocket* Protocol::getSocket()
{
    return memberSocket;
}

QStringList makeSendData(QString header, QString event, QString PID, QString data)
{
    QStringList sendData;
    sendData << header << event << PID << data;
    return sendData;
}

void Protocol::sendProtocolToServer(QStringList sendData)
{
    QByteArray dataArray;
    QDataStream out(&dataArray, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << sendData;
    memberSocket->write(dataArray);
    memberSocket->flush();
    while(memberSocket->waitForBytesWritten());
}

QStringList Protocol::parsingPacket(QTcpSocket* socket)
{
    memberSocket = socket;
    QByteArray receiveArray = memberSocket->readAll();
    QString receiveData = QString::fromUtf8(receiveArray.toStdString().c_str());
    QStringList data = receiveData.split("<CR>");
    return data;
}

void Protocol::sendProtocolToClient(QTcpSocket *socket, QStringList sendData)
{

}
