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
//        connect(fileSocket, SIGNAL(readyRead()), SLOT(receiveFile()));
        protocol->sendProtocol(fileSocket, "NEW", ConnectType::MODALITY, "MODALITY");
    } else {
        // 연결 실패  예외처리 구현
    }
}

Widget::~Widget()
{
}

void Widget::buttonClicked()
{
    sendFile();
}

void Widget::sendFile()
{
    QFile file;
    QString fileName;

    for (int i = 10; i < 999; i++) {
        if (i >= 100)
            fileName = QString("./CEPH/0%1.raw").arg(i);
        else
            fileName = QString("./CEPH/00%1.raw").arg(i);

        file.setFileName(fileName);
        file.open(QIODevice::ReadOnly);
        totalData.append(file.readAll());
        file.close();
    }
    qDebug("TOTAL SIZE : %d", totalData.size());
    fileSocket->write(totalData);
    fileSocket->flush();
    totalData.clear();

}
