#include "protocol.h"

Protocol::Protocol()
{
    memberSocket = new QTcpSocket();
}

Protocol::Protocol(QTcpSocket *socket)
{
    memberSocket = new QTcpSocket();
    memberSocket = socket;
}

Protocol::~Protocol()
{

}

void Protocol::setSocket(QTcpSocket *socket)
{
    memberSocket = socket;
}

QTcpSocket* Protocol::getSocket()
{
    return memberSocket;
}


void Protocol::sendProtocolToServer(QTcpSocket *socket, Type type, QString header, QString event, QString msg, QString PID)
{
    QStringList dataList;
    dataList << header << event << msg << PID;

    QByteArray dataArray;
    QDataStream out(&dataArray, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << type;
    out << makeProtocolData(dataList);
    socket->write(dataArray);
    socket->flush();
    while(socket->waitForBytesWritten());
}

QString Protocol::makeProtocolData(QStringList dataList)
{
    return dataList[0] + "^" + dataList[1] + "<CR>" + dataList[2] + "<CR>" + dataList[3] + "<END>";
}
