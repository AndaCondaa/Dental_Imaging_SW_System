#include "protocol.h"

#include <QTcpSocket>
#include <QDataStream>

void sendProtocol(QTcpSocket *socket, Type type, QString header, QString event, QString msg, QString PID)
{
    QStringList dataList;
    dataList << header << event << msg << PID;
    QString sendData = makeProtocolData(dataList);

    QByteArray dataArray;
    QDataStream out(&dataArray, QIODevice::WriteOnly);
    out.device()->seek(0);
    out << sendData;
    socket->write(dataArray);
    socket->flush();
    while(socket->waitForBytesWritten());
}

QString makeProtocolData(QStringList dataList)
{
//    QString sendData;
//    sendData << dataList[0]
//    sendData << "^";
//    sendData << dataList[1];
//    sendData << "<CR>";
//    sendData << dataList[2];
//    sendData << "<CR>";
//    sendData << dataList[3];
//    sendData << "<END>";
}
