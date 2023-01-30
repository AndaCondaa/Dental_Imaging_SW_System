#include "widget.h"
#include <QTcpSocket>
#include <QPushButton>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QPushButton *button = new QPushButton(this);
    button->setGeometry(50, 50, 100, 100);
    connect(button, SIGNAL(clicked()), this, SLOT(buttonClicked()));

    protocol = new Protocol;

    fileSocket = new QTcpSocket;
    fileSocket->connectToHost("127.0.0.1", 8003);
    if (fileSocket->waitForConnected()) {
        connect(fileSocket, SIGNAL(readyRead()), SLOT(receiveFile()));
        protocol->sendProtocol(fileSocket, "NEW", ConnectType::MODALITY, "MODALITY");
    } else {
        // 연결 실패  예외처리 구현
    }
}

Widget::~Widget()
{
}

void Widget::receiveFile()
{
    qDebug("%d", __LINE__);
}

void Widget::buttonClicked()
{
    connect(fileSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
    count = 10;
    sendFile(count);
}

void Widget::sendFile(int num)
{
    QString fileName;
    if (num >= 100)
        fileName = QString("./CEPH/0%1.raw").arg(num);
    else
        fileName = QString("./CEPH/00%1.raw").arg(num);

    loadSize = 0;
    byteToWrite = 0;
    totalSize = 0;
    outBlock.clear();

    if(fileName.length()) {
        file = new QFile(fileName);
        file->open(QFile::ReadOnly);

        qDebug() << QString("file %1 is opened").arg(fileName);

        byteToWrite = totalSize = file->size(); // Data remained yet
        loadSize = 1024; // Size of data per a block

        QDataStream out(&outBlock, QIODevice::WriteOnly);
        out << qint64(0) << qint64(0) << fileName;

        totalSize += outBlock.size();
        byteToWrite += outBlock.size();

        out.device()->seek(0);
        out << totalSize << qint64(outBlock.size());

        fileSocket->write(outBlock); // Send the read file to the socket
    }
    qDebug() << QString("Sending file %1").arg(fileName);
}

void Widget::goOnSend(qint64 numBytes)
{
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    byteToWrite -= numBytes; // Remaining data size
    outBlock = file->read(qMin(byteToWrite, numBytes));
    socket->write(outBlock);

    if (byteToWrite == 0) { // Send completed
        if (count < 998) {
            count++;
            sendFile(count);
        }
    }
}
