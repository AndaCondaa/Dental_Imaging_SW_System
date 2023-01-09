#include "protocol.h"

void sendProtocolToServer(QTcpSocket *socket, Type type, QString header, QString event, QString msg, QString PID)
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

QString makeProtocolData(QStringList dataList)
{
    return dataList[0] + "^" + dataList[1] + "<CR>" + dataList[2] + "<CR>" + dataList[3] + "<END>";
}
